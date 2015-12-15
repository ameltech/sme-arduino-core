/*
 Shows The result of the sme initialization phase

 created 16 Jun 2015
 by Mik (smkk@amel-tech.com)
 */

#include <Arduino.h>

void setup() {
    // LED & User Button are already initialized by the SME core.
    // it is not required to do here

    SerialUSB.begin(115200);
}

void loop() {

  SerialUSB.print("SmartEverything initialization = ");

  if (smeInitError == 0){
    SerialUSB.println("SUCCEED !");
    ledGreenLight(HIGH);
  }    
  else {
    //  ERROR !!!!!!! FLASH EVERYTHING

    SerialUSB.println("ERROR !");

        pinMode(PIN_LED_RXL, OUTPUT);
        pinMode(PIN_LED_TXL, OUTPUT);
        int i=0;

        while(1){
            switch (i) {
                case 0:
                i++;
                ledGreenLight(HIGH);
                break;

                case 1:
                i++;
                ledRedLight(HIGH);
                break;

                case 2:
                i=0;
                ledBlueLight(HIGH);
                break;

                default:
                break;
            }
            digitalWrite(PIN_LED_RXL, LOW);
            digitalWrite(PIN_LED_TXL, LOW);
            delay(100);


            ledGreenLight(LOW);
            ledBlueLight(LOW);
            ledRedLight(LOW);
            digitalWrite(PIN_LED_RXL, HIGH);
            digitalWrite(PIN_LED_TXL, HIGH);
            delay(100);
        };
  }    

    delay(2000);

}

