/*
 * internalI2C.h
 *
 * Created: 10/9/2015 11:22:23 PM
 *  Author: smkk
 */ 


#ifndef INTERNALI2C_H_
#define INTERNALI2C_H_

#define BUFFER_LENGTH 32

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


// Wire initialization function
void internalI2CInit(void);

byte readRegister(byte _address, byte regToRead);

// Writes a single byte (dataToWrite) into regToWrite
bool writeRegister(byte _address, byte regToWrite, byte dataToWrite);


#endif /* INTERNALI2C_H_ */