/*
Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
Copyright (c) 2015 Amel-Tech (a division of Amel Srl). All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h>
#include "WireIoExt.h"

static const uint32_t TWI_CLOCK_SME = 100000;

#define HTS221_ADDRESS     0x5F
//Define a few of the registers that we will be accessing on the HTS221
#define WHO_AM_I           0x0F
#define WHO_AM_I_RETURN    0xBC //This read-only register contains the device identifier, set to BCh


#define TCA6416A_ADDRESS 0x20
#define CONF_PORT_0     0b01111111
#define INIT_P0         0b10000000

#define CONF_PORT_1     0b00110101
#define INIT_P1         0b11001010
#define VALUE_GPS_FORCE_ON   0b10001010

// Protocol          PowerOn Default
#define INPUT_PORT_0    0x00 //Read byte          xxxx xxxx (undefined)
#define INPUT_PORT_1    0x01 //Read byte          xxxx xxxx
#define OUTPUT_PORT_0   0x02 //Read/write byte    1111 1111
#define OUTPUT_PORT_1   0x03 //Read/write byte    1111 1111
#define P_INVERT_PORT_0 0x04 //Read/write byte    0000 0000
#define P_INVERT_PORT_1 0x05 //Read/write byte    0000 0000
#define CONFIG_PORT_0   0x06 //Read/write byte    1111 1111
#define CONFIG_PORT_1   0x07 //Read/write byte    1111 1111


#define _9AX_INT2_A_G_PIN   0x1
#define _9AX_INT1_A_G_PIN   0x2
#define _9AX_INT_M_PIN      0x4
#define _9AX_DRDY_M_PIN     0x8
#define PRE_INT_PIN         0x10
#define ALS_GPIO0_PIN       0x20
#define ALS_GPIO1_PIN       0x40
#define BLE_RESET_PIN       0x80

#define SFX_STDBY_STS_PIN   0x1
#define SFX_RESET_PIN       0x2
#define SFX_RADIO_STS_PIN   0x4
#define SFX_WAKEUP_PIN      0x8
#define NFC_FD_PIN         0x10
#define HUT_DRDY_PIN        0x20
#define GPS_FORCE_ON_PIN    0x40
#define GPS_RESET_PIN       0x80


static byte readRegister(byte _address, byte regToRead)
{
    WireTemp.beginTransmission(_address);
    WireTemp.write(regToRead);
    WireTemp.endTransmission(false); //endTransmission but keep the connection active

    WireTemp.requestFrom(_address, 1); //Ask for 1 byte, once done, bus is released by default

    while(!WireTemp.available()) ; //Wait for the data to come back
    return WireTemp.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into regToWrite
static bool writeRegister(byte _address, byte regToWrite, byte dataToWrite)
{
    WireTemp.beginTransmission(_address);

    if (!WireTemp.write(regToWrite)) {
        return false;
    }
    if (!WireTemp.write(dataToWrite)) {
        return false;
    }

    uint8_t errorNo = WireTemp.endTransmission(); //Stop transmitting
    return (errorNo == 0);
}

uint8_t resetsDbg[2];
uint8_t actual[2];

uint8_t smeInitError=0xFF;
static void ioExtenderInit(void) {
    WireTemp.begin();


    // first test the I2C bus
    uint8_t data = readRegister(HTS221_ADDRESS, WHO_AM_I);
    if (data == WHO_AM_I_RETURN){
        smeInitError = 0;

        if (writeRegister( TCA6416A_ADDRESS, CONFIG_PORT_0, CONF_PORT_0)!=false) {
            writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0, INIT_P0); // keep the BLE reset High
        }

        if (writeRegister( TCA6416A_ADDRESS, CONFIG_PORT_1, CONF_PORT_1)!=false){
            writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1, INIT_P1); // keep the resets High
        }

        // wait a while
        delay(20);
        resetsDbg[0]=0;
        resetsDbg[1]=0;


        actual[0] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0);
        writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0, (actual[0]&~BLE_RESET_PIN));
        resetsDbg[0] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0);

        if ((actual[1] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1)) != false) {
            writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1, (actual[1]&(~GPS_RESET_PIN) &(~SFX_WAKEUP_PIN)));
            resetsDbg[1] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1);
        }

        // wait a while
            delay(20);

            // move the reset high
            writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0, actual[0]);
            resetsDbg[0] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_0);

            writeRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1, actual[1]);
            resetsDbg[1] = readRegister( TCA6416A_ADDRESS, OUTPUT_PORT_1);

    } else {

        smeInitError = 1;
    }

}


void initVariant() {
    // reset the I/O Extender
    pinMode(PIN_IO_EXT_RST, OUTPUT);
    digitalWrite(PIN_IO_EXT_RST, LOW);
    delay(10); // just wait a while
    digitalWrite(PIN_IO_EXT_RST, HIGH);

    // initialize button1 & button2 as input
    pinMode(PIN_SME_BUTTON1, INPUT_PULLUP);
    pinMode(PIN_SME_BUTTON2, INPUT_PULLUP);


    // initialize RGB LED
    LED_GREEN_INIT;
    LED_RED_INIT;
    LED_BLUE_INIT;
    ledGreenLight(LOW);
    ledBlueLight(LOW);
    ledRedLight(LOW);

    // initialize The EXT_PWR Pin as input
    // it will be HIGH when the battery is connected
    pinMode(PIN_EXT_PWR, INPUT);
    // initialize the SetpUp to Disable
    pinMode(PIN_REG_ON, OUTPUT);
    setStepUp(LOW);


    // initialize the IO_Extender
    ioExtenderInit();
}
