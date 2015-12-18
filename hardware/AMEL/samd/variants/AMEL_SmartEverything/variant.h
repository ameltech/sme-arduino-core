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

#ifndef _VARIANT_AMEL_SMARTEVERYTHING_
#define _VARIANT_AMEL_SMARTEVERYTHING_

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		(32768ul)

/** Master clock frequency */
#define VARIANT_MCK			  (48000000ul)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"
#include <stdbool.h>

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (26u)
#define NUM_DIGITAL_PINS     (14u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (1u)

#define digitalPinToPort(P)        ( &(PORT->Group[g_APinDescription[P].ulPort]) )
#define digitalPinToBitMask(P)     ( 1 << g_APinDescription[P].ulPin )
//#define analogInPinToBit(P)        ( )
#define portOutputRegister(port)   ( &(port->OUT.reg) )
#define portInputRegister(port)    ( &(port->IN.reg) )
#define portModeRegister(port)     ( &(port->DIR.reg) )
#define digitalPinHasPWM(P)        ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )

/*
 * digitalPinToTimer(..) is AVR-specific and is not defined for SAMD
 * architecture. If you need to check if a pin supports PWM you must
 * use digitalPinHasPWM(..).
 *
 * https://github.com/arduino/Arduino/issues/1833
 */
// #define digitalPinToTimer(P)

// Interrupts
#define digitalPinToInterrupt(P)   ( g_APinDescription[P].ulExtInt )

// LEDs
#define PIN_LED_RED          (45u)
#define PIN_LED_GREEN        (46u)
#define PIN_LED_BLUE         (47u)
#define PIN_LED_13           PIN_LED_GREEN
#define PIN_LED_RXL          (26u)
#define PIN_LED_TXL          (27u)
#define PIN_LED              PIN_LED_13
#define PIN_LED2             PIN_LED_RXL
#define PIN_LED3             PIN_LED_TXL
#define LED_BUILTIN          PIN_LED_13

// I/O Extender
#define PIN_IO_EXT_RST      (48u)
#define PIN_IO_EXT_INT      (49u)


// Step-Up Power
#define PIN_EXT_PWR         (50u)
#define PIN_REG_ON          (51u)

// User Button
#define PIN_SME_BUTTON1        (41u)
#define PIN_SME_BUTTON2        (42u)


/*
 * Analog pins
 */
#define PIN_A0               (14ul)
#define PIN_A1               (15ul)
#define PIN_A2               (16ul)
#define PIN_A3               (17ul)
#define PIN_A4               (18ul)
#define PIN_A5               (19ul)

static const uint8_t A0  = PIN_A0 ;
static const uint8_t A1  = PIN_A1 ;
static const uint8_t A2  = PIN_A2 ;
static const uint8_t A3  = PIN_A3 ;
static const uint8_t A4  = PIN_A4 ;
static const uint8_t A5  = PIN_A5 ;
#define ADC_RESOLUTION		12

/*
 * Serial interfaces
 */
// GPS
#define PIN_GPS_TX           (31ul)
#define PIN_GPS_RX           (32ul)
#define PAD_GPS_TX           (UART_TX_PAD_0)
#define PAD_GPS_RX           (SERCOM_RX_PAD_1)

// BLE
#define PIN_BLE_TX           (37ul)
#define PIN_BLE_RX           (38ul)
#define PIN_BLE_RTS          (39ul)
#define PIN_BLE_CTS          (40ul)
#define PAD_BLE_TX           (UART_TX_RTS_CTS_PAD_0_2_3)
#define PAD_BLE_RX           (SERCOM_RX_PAD_1)

// SigFox
#define PIN_SIGFOX_RX        (33ul)
#define PIN_SIGFOX_TX        (34ul)
#define PIN_SIGFOX_RTS       (35ul)
#define PIN_SIGFOX_CTS       (36ul)
#define PAD_SIGFOX_TX        (UART_TX_RTS_CTS_PAD_0_2_3)
#define PAD_SIGFOX_RX        (SERCOM_RX_PAD_1)

// Serial1
#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)
#define PAD_SERIAL1_TX       (UART_TX_PAD_2)
#define PAD_SERIAL1_RX       (SERCOM_RX_PAD_3)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (22u)
#define PIN_SPI_MOSI         (23u)
#define PIN_SPI_SCK          (24u)
#define PERIPH_SPI           sercom4
#define PAD_SPI_TX           SPI_PAD_2_SCK_3
#define PAD_SPI_RX           SERCOM_RX_PAD_0

#define PIN_SPI_SS           (25u)

static const uint8_t SS	  = PIN_SPI_SS ;
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)
#define PERIPH_WIRE          sercom3
#define WIRE_IT_HANDLER      SERCOM3_Handler

/*
 * USB
 */
#define PIN_USB_HOST_ENABLE  (28ul)
#define PIN_USB_DM           (29ul)
#define PIN_USB_DP           (30ul)

/*
    RGB wrapper function
    These functions has been created for a more comfortable use 
      because internally wrap the inversion of the HIGH, LOW meaning.
    Using these function it remain the same Arduino User Experience to light a led.

    parameter:
    value = the light intensity. It could be 
            HIGH   = light to the maximum level
            LOW    = switch off the Led
            1..255 = pwm value for different level of light
 */
void ledGreenLight(uint32_t value);
void ledRedLight(uint32_t value);
void ledBlueLight(uint32_t value);

/*
    Wrapper to flash the RGB Led light red or blue or green for X milliseconds
*/
void flashRGBLed(uint32_t color, uint32_t time_in_ms);

/*
    Yellow Led wrapper function
    These functions has been created for a more comfortable use 
      because internally wrap the inversion of the HIGH, LOW meaning

    Using these function it remain the same Arduino User Experience to light a led
    HIGH = Light ON
    LOW  = Light OFF

 */
void ledYellowOneLight(uint32_t value);
void ledYellowTwoLight(uint32_t value);

/*
    User Button wrapper function.

    return:
    1 = button PRESSED
    0 = button RELEASED
*/
int isButtonOnePressed(void);
int isButtonTwoPressed(void);


/*
    Enable/Disable the StepUp
    
    param:
    true = Step-up the Battery Power
    false= Do not Step-up the Battery Power
*/
void setStepUp(uint32_t on);

/*
    Return the information if the StepUp is enabled
    
    return:
    true= the StepUp is running
    false = the Sme is not under StepUp Power
*/
bool isOnBattery(void);


/*
 * FORCE-ON (only SE868-A)
Force-ON is an input signal that can be used to wake up the SE868-A from the sleep mode. 
It is internally pulled-up. It has active-low logic, i.e. the module wakes up when FORCE_ON is tied to ground. When inactive, it should be left open drain or open collector.

Note:
keeping FORCE_ON tied to ground won’t prevent the SE868-A from going into sleep mode, since this signal is sensitive only to the high-low transition.
No pull-up circuits are allowed on the FORCE_ON pin, since the signal is already internally pulled up.
*/
void gpsForceOn(void);


void sfxSleep(void);
void sfxWakeup(void);

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/


#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
 */
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;

extern Uart Serial1;

extern Uart GPS;
extern Uart BLE;
extern Uart SigFox;

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_MONITOR         SerialUSB
// Serial has no physical pins broken out, so it's not listed as HARDWARE port
#define SERIAL_PORT_HARDWARE        Serial1
#define SERIAL_PORT_HARDWARE_OPEN   Serial1


#define LED_GREEN_INIT  pinMode(PIN_LED_GREEN, OUTPUT)
#define LED_RED_INIT    pinMode(PIN_LED_RED, OUTPUT)
#define LED_BLUE_INIT   pinMode(PIN_LED_BLUE, OUTPUT)

#define LED_YELLOW_TWO_INIT  pinMode(PIN_LED_RXL, OUTPUT)
#define LED_YELLOW_ONE_INIT  pinMode(PIN_LED_TXL, OUTPUT)


extern uint8_t smeInitError;
#endif /* _VARIANT_AMEL_SMARTEVERYTHING_ */

