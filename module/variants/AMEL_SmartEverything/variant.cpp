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
#include "internalI2C.h"

/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[]=
{
/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * + Pin number + SmartEverything  |  PIN   | Label/Name      | Comments (* is for default peripheral in use)
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Digital Low      |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 0          | 0 <- RX          |  PA11  |                 | EIC/EXTINT[11] ADC/AIN[19]           PTC/X[3] *SERCOM0/PAD[3]  SERCOM2/PAD[3]  TCC0/WO[3]  TCC1/WO[1]
 * | 1          | 1 -> TX          |  PA10  |                 | EIC/EXTINT[10] ADC/AIN[18]           PTC/X[2] *SERCOM0/PAD[2]  SERCOM2/PAD[2]  TCC0/WO[2]  TCC1/WO[0]
 * | 2          | ~2               |  PB31  |                 | EIC/EXTINT[15]                                                 SERCOM5/PAD[1] *TCC0/WO[1]  TCC1/WO[3]
 * | 3          | ~3               |  PB30  |                 | EIC/EXTINT[14]                                                 SERCOM5/PAD[0] *TCC0/WO[0]  TCC1/WO[2]
 * | 4          | ~4               |  PA14  |                 | EIC/EXTINT[14]                                 SERCOM2/PAD[2]  SERCOM4/PAD[2] *TC3/WO[0]   TCC0/WO[4]
 * | 5          | ~5               |  PA15  |                 | EIC/EXTINT[15]                                 SERCOM2/PAD[3]  SERCOM4/PAD[3] *TC3/WO[1]   TCC0/WO[5]
 * | 6          | ~6               |  PA20  |                 | EIC/EXTINT[4]                        PTC/X[8]  SERCOM5/PAD[2]  SERCOM3/PAD[2] *TC7/WO[0]   TCC0/WO[6]
 * | 7          | ~7               |  PA21  |                 | EIC/EXTINT[5]                        PTC/X[9]  SERCOM5/PAD[3]  SERCOM3/PAD[3] *TC7/WO[1]   TCC0/WO[7]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  // SERCOM/UART (Serial1)
  { PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM2/PAD[3]
  { PORTA, 10, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10 }, // TX: SERCOM2/PAD[2]

  { PORTB, 31, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_15 }, // TCC0/WO[1]
  { PORTB, 30, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_14 }, // TCC0/WO[0]
  { PORTA, 14, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH0, TC3_CH0, EXTERNAL_INT_14 }, // TC3/WO[0]
  { PORTA, 15, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH1, TC3_CH1, EXTERNAL_INT_15 }, // TC3/WO[1]
  { PORTA, 20, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM7_CH0, TC7_CH0, EXTERNAL_INT_4 }, // TC7/WO[0]
  { PORTA, 21, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM7_CH0, TC7_CH1, EXTERNAL_INT_5 }, // TC7/WO[1]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Digital High     |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 8          | ~8               |  PA06  |                 | EIC/EXTINT[6]  ADC/AIN[6]  AC/AIN[2] PTC/Y[4]  SERCOM0/PAD[2]                 *TCC1/WO[0]
 * | 9          | ~9               |  PA07  |                 | EIC/EXTINT[7]  ADC/AIN[7]  AC/AIN[3] PTC/Y[5]  SERCOM0/PAD[3]                 *TCC1/WO[1]
 * | 10         | ~10              |  PA18  |                 | EIC/EXTINT[2]                        PTC/X[6] +SERCOM1/PAD[2]  SERCOM3/PAD[2]  TC3/WO[0]   *TCC0/WO[2]
 * | 11         | ~11              |  PA16  |                 | EIC/EXTINT[0]                        PTC/X[4] +SERCOM1/PAD[0]  SERCOM3/PAD[0] *TCC2/WO[0]   TCC0/WO[6]
 * | 12         | ~12              |  PA19  |                 | EIC/EXTINT[3]                        PTC/X[7] +SERCOM1/PAD[3]  SERCOM3/PAD[3]  TC3/WO[1]   *TCC0/WO[3]
 * | 13         | ~13              |  PA17  |                 | EIC/EXTINT[1]                        PTC/X[5] +SERCOM1/PAD[1]  SERCOM3/PAD[1] *TCC2/WO[1]   TCC0/WO[7]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
*/
  { PORTA,  6, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_4 }, // TCC1/WO[0]
  { PORTA,  7, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM1_CH1, TCC1_CH1, EXTERNAL_INT_5 }, // TCC1/WO[1]
  { PORTA, 18, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH2, TCC0_CH2, EXTERNAL_INT_2 }, // TCC0/WO[2]
  { PORTA, 16, PIO_DIGITAL, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_0 }, // PIO
  { PORTA, 19, PIO_TIMER_ALT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM0_CH3, TCC0_CH3, EXTERNAL_INT_3 }, // TCC0/WO[3]
  { PORTA, 17, PIO_TIMER, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH1, TCC2_CH1, EXTERNAL_INT_1 }, // TCC2/WO[1]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Analog Connector |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 14         | A0               |  PA02  | A0              | EIC/EXTINT[2] *ADC/AIN[0]  DAC/VOUT  PTC/Y[0]
 * | 15         | A1               |  PB08  | A1              | EIC/EXTINT[8] *ADC/AIN[2]            PTC/Y[14] SERCOM4/PAD[0]                  TC4/WO[0]
 * | 16         | A2               |  PB09  | A2              | EIC/EXTINT[9] *ADC/AIN[3]            PTC/Y[15] SERCOM4/PAD[1]                  TC4/WO[1]
 * | 17         | A3               |  PA04  | A3              | EIC/EXTINT[4] *ADC/AIN[4]  AC/AIN[0] PTC/Y[2]  SERCOM0/PAD[0]                  TCC0/WO[0]
 * | 18         | A4               |  PA05  | A4              | EIC/EXTINT[5] *ADC/AIN[5]  AC/AIN[1] PTC/Y[5]  SERCOM0/PAD[1]                  TCC0/WO[1]
 * | 19         | A5               |  PB02  | A5              | EIC/EXTINT[2] *ADC/AIN[10]           PTC/Y[8]  SERCOM5/PAD[0]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA,  2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // ADC/AIN[0]
  { PORTB,  8, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_8 }, // ADC/AIN[2]
  { PORTB,  9, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel3, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_9 }, // ADC/AIN[3]
  { PORTA,  4, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel4, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_4 }, // ADC/AIN[4]
  { PORTA,  5, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel5, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5 }, // ADC/AIN[5]
  { PORTB,  2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel10, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // ADC/AIN[10]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Wire             |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 20         | SDA              |  PA22  | SDA             | EIC/EXTINT[6]                        PTC/X[10] *SERCOM3/PAD[0] SERCOM5/PAD[0] TC4/WO[0] TCC0/WO[4]
 * | 21         | SCL              |  PA23  | SCL             | EIC/EXTINT[7]                        PTC/X[11] *SERCOM3/PAD[1] SERCOM5/PAD[1] TC4/WO[1] TCC0/WO[5]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA, 22, PIO_SERCOM, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SDA: SERCOM3/PAD[0]
  { PORTA, 23, PIO_SERCOM, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7 }, // SCL: SERCOM3/PAD[1]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Digital High     |        |                 |
 * |            |     SPI          |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 22         | MISO             |  PA19  |                 | SERCOM1/PAD[3]
 * | 23         | MOSI             |  PA16  |                 | SERCOM1/PAD[0]
 * | 24         | SCK              |  PA17  |                 | SERCOM1/PAD[1]
 * | 25         | SS               |  PA18  |                 | SERCOM1/PAD[2]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA, 19, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // MISO: SERCOM1/PAD[3]
  { PORTA, 16, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // MOSI: SERCOM1/PAD[0]
  { PORTA, 17, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SCK: SERCOM1/PAD[1]
  { PORTA, 18, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SS: SERCOM1/PAD[2]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | LEDs             |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 26         |                  |  PB03  | RX              |
 * | 27         |                  |  PB01  | TX              |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTB,  3, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // used as output only
  { PORTB,  1, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // used as output only

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | USB              |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 28         |                  |  PA28  | USB_HOST_ENABLE | EIC/EXTINT[8]
 * | 29         |                  |  PA24  | USB_NEGATIVE    | *USB/DM
 * | 30         |                  |  PA25  | USB_POSITIVE    | *USB/DP
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA, 28, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB Host enable
  { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
  { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DP

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | GPS              |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 31         |                  |  PA08  | GPS_UART TX     | *SERCOM2/PAD[0]
 * | 32         |                  |  PA09  | GPS_UART RX     | *SERCOM2/PAD[1]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA,  8, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TX: SERCOM2/PAD[0]
  { PORTA,  9, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // RX: SERCOM2/PAD[1]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | SigFox           |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 33         |                  |  PB12  | SFX_TXD         | *SERCOM4/PAD[0]
 * | 34         |                  |  PB13  | SFX_RXD         | *SERCOM4/PAD[1]
 * | 35         |                  |  PB14  | SFX_RTS         | *SERCOM4/PAD[2]
 * | 36         |                  |  PB15  | SFX_CTS         | *SERCOM4/PAD[3]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTB, 12, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TX: SERCOM4/PAD[0]
  { PORTB, 13, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // RX: SERCOM4/PAD[1]
  { PORTB, 14, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // RTS: SERCOM4/PAD[2]
  { PORTB, 15, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // CTS: SERCOM4/PAD[3]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | BLE              |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 37         |                  |  PB16  | BLE_TXD         | *SERCOM5/PAD[0]
 * | 38         |                  |  PB17  | BLE_RXD         | *SERCOM5/PAD[1]
 * | 39         |                  |  PB22  | BLE_RTS         | *SERCOM5/PAD[2]
 * | 40         |                  |  PB23  | BLE_CTS         | *SERCOM5/PAD[3]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTB, 16, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TX: SERCOM5/PAD[0]
  { PORTB, 17, PIO_SERCOM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // RX: SERCOM5/PAD[1]
  { PORTB, 22, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // RTS: SERCOM5/PAD[2]
  { PORTB, 23, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // CTS: SERCOM5/PAD[3]

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Push Buttons     |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 41         |                  |  PB00  | PUSHB_1         |
 * | 42         |                  |  PA27  | PUSHB_2         |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTB,  0, PIO_INPUT_PULLUP, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // used as input only
  { PORTA, 27, PIO_INPUT_PULLUP, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // used as input only

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Digital High     |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 43         | AREF             |  PA03  |                 | EIC/EXTINT[3] *[ADC|DAC]/VREFA ADC/AIN[1] PTC/Y[1]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA, 3, PIO_ANALOG, PIN_ATTR_ANALOG, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // DAC/VREFP

/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Analog Connector |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 44         | A0/DAC           |  PA02  |                 | DAC/VOUT
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA,  2, PIO_ANALOG, PIN_ATTR_ANALOG, DAC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // DAC/VOUT
    
/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | RGB LED          |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 45         | RGB_RED          |  PA12  |                 | *TCC2/WO[0]
 * | 46         | RGB_GREEN        |  PB11  |                 | *TC5/WO[1]
 * | 47         | RGB_BLUE         |  PB10  |                 | *TC5/WO[0]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
  { PORTA, 12, PIO_PWM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), No_ADC_Channel, PWM2_CH0, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TCC2/WO[0]
  { PORTB, 11, PIO_PWM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), No_ADC_Channel, PWM5_CH1, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TC5/WO[1]
  { PORTB, 10, PIO_PWM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM), No_ADC_Channel, PWM5_CH0, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // TC5/WO[1]
            
/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | I/O Extension    |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 48         | IO_EXT_RST       |  PA13  |                 | EXTINT[13] SERCOM2/ PAD[1] SERCOM4/ PAD[1] TCC2/WO[1] TCC0/ WO[7] AC/CMP[1]
 * | 49         | IO_EXT_INT       |  PB06  |                 | EXTINT[6] AIN[14] Y[12]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */
 { PORTA, 13, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // I2C extender reset
 { PORTB, 06, PIO_EXTINT, PIN_ATTR_EXTINT, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6 }, // I2C extender interrupt
      
/*
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * |            | Step-up PWR      |        |                 |
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 * | 50         | EXT_PWR          |  PB04  |                 | EXTINT[4] AIN[12] Y[10]
 * | 51         | REG_ON           |  PB05  |                 | EXTINT[5] AIN[13] Y[11]
 * +------------+------------------+--------+-----------------+--------------------------------------------------------------------------------------------------------
 */      
  { PORTB,  4, PIO_INPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_4},  // used as input only
  { PORTB,  5, PIO_OUTPUT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5 }, // used as output only
} ;


const void* g_apTCInstances[TCC_INST_NUM+TC_INST_NUM]={ TCC0, TCC1, TCC2, TC3, TC4, TC5, TC6, TC7 } ;

// Multi-serial objects instantiation
SERCOM sercom0( SERCOM0 ) ;
SERCOM sercom1( SERCOM1 ) ;
SERCOM sercom2( SERCOM2 ) ;
SERCOM sercom3( SERCOM3 ) ;
SERCOM sercom4( SERCOM4 ) ;
SERCOM sercom5( SERCOM5 ) ;

Uart Serial1( &sercom0, PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX ) ;

Uart GPS( &sercom2, PIN_GPS_RX, PIN_GPS_TX, PAD_GPS_RX, PAD_GPS_TX ) ;
Uart BLE( &sercom5, PIN_BLE_RX, PIN_BLE_TX, PAD_BLE_RX, PAD_BLE_TX ) ;
Uart SigFox( &sercom4, PIN_SIGFOX_RX, PIN_SIGFOX_TX, PAD_SIGFOX_RX, PAD_SIGFOX_TX ) ;

void SERCOM0_Handler()
{
  Serial1.IrqHandler();
}

void SERCOM2_Handler()
{
  GPS.IrqHandler();
}

void SERCOM4_Handler()
{
  SigFox.IrqHandler();
}

void SERCOM5_Handler()
{
  BLE.IrqHandler();
}

void ledYellowTwoLight(uint32_t value) {
    if (value == HIGH) {
        digitalWrite(PIN_LED_RXL, LOW);
        } else if (value == LOW) {
        digitalWrite(PIN_LED_RXL, HIGH);
        } else {
        analogWrite(PIN_LED_RXL, 255-value);   // in case of PWM
    }
}

void ledYellowOneLight(uint32_t value)   {
    if (value == HIGH) {
        digitalWrite(PIN_LED_TXL, LOW);
        } else if (value == LOW) {
        digitalWrite(PIN_LED_TXL, HIGH);
        } else {
        analogWrite(PIN_LED_TXL, 255-value);   // in case of PWM
    }
}

void ledGreenLight(uint32_t value) {
    if (value == HIGH) {
        digitalWrite(PIN_LED_GREEN, LOW);
    } else if (value == LOW) {
        digitalWrite(PIN_LED_GREEN, HIGH);
    } else {
        analogWrite(PIN_LED_GREEN, 255-value);   // in case of PWM
    }
}

void ledRedLight(uint32_t value)   {
    if (value == HIGH) {
        digitalWrite(PIN_LED_RED, LOW);
    } else if (value == LOW) {
        digitalWrite(PIN_LED_RED, HIGH);
    } else {
        analogWrite(PIN_LED_RED, 255-value);   // in case of PWM
    }
}

void ledBlueLight(uint32_t value) {
    if (value == HIGH) {
        digitalWrite(PIN_LED_BLUE, LOW);
    } else if (value == LOW) {
        digitalWrite(PIN_LED_BLUE, HIGH);
    } else {
        analogWrite(PIN_LED_BLUE, 255-value);   // in case of PWM
    }
}

void flashRGBLed(uint32_t color, uint32_t time_in_ms) {
    // If the color is not a valid one, do nothing
    if (color == PIN_LED_GREEN || color == PIN_LED_BLUE || color == PIN_LED_RED) {
        digitalWrite(color, LOW);
        delay(time_in_ms);
        digitalWrite(color, HIGH);
    }
}

int isButtonOnePressed(void) {
    return !digitalRead(PIN_SME_BUTTON1);
}


int isButtonTwoPressed(void) {
    return !digitalRead(PIN_SME_BUTTON2);
}


bool isOnBattery(void) {   
    return !digitalRead(PIN_EXT_PWR);
}


void setStepUp(uint32_t on) {
    if (on) {
        digitalWrite(PIN_REG_ON, HIGH);
        } else {
        digitalWrite(PIN_REG_ON, LOW);
    }
}


void gpsForceOn(void){
    bool ret = false;
    uint8_t delay=0;
    uint8_t data = 0;
    
	// Activate force on moving low
    data = readRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1);
    data &= ~GPS_FORCE_ON_PIN;
    
	writeRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1, data);

}
void sfxSleep(void){
    bool ret = false;
    uint8_t delay=0;
    uint8_t data = 0;
    
	// read the IoExtender data actual situation
    data = readRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1);
    data |= SFX_WAKEUP_PIN; // Put SFX in Sleep
    
	// send to IoExtender the new data
	writeRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1, data);

}

void sfxWakeup(void){
     bool ret = false;
     uint8_t delay=0;
     uint8_t data = 0;
	 
     // read the IoExtender data actual situation
     data = readRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1);
     data &= ~SFX_WAKEUP_PIN;   // Wakeup SFX
         
     // send to IoExtender the new data
     writeRegister(TCA6416A_ADDRESS, OUTPUT_PORT_1, data);
}
