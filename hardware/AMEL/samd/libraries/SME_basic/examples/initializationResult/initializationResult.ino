/*
 Shows The result of the sme initialization phase

 created 16 Jun 2015
 by Mik (smkk@amel-tech.com)
 */

#include <Arduino.h>

void setup() {
    SerialUSB.begin(115200);
}

void loop() {

  SerialUSB.print("SmartEverything initialization = ");

  if (smeInitError == 0){
    SerialUSB.println("SUCCEED !");
    LED_GREEN_ON(HIGH);
  }    
  else {
    SerialUSB.println("ERROR !");
    //         // ERROR !!!!!!! FLASH EVERYTHING

        pinMode(PIN_LED_RXL, OUTPUT);
        pinMode(PIN_LED_TXL, OUTPUT);
        int i=0;

        while(1){
            switch (i) {
                case 0:
                i++;
                LED_GREEN_ON(HIGH);
                break;

                case 1:
                i++;
                LED_RED_ON(HIGH);
                break;

                case 2:
                i=0;
                LED_BLUE_ON(HIGH);
                break;

                default:
                break;
            }
            digitalWrite(PIN_LED_RXL, LOW);
            digitalWrite(PIN_LED_TXL, LOW);
            delay(100);


            LED_GREEN_ON(LOW);
            LED_BLUE_ON(LOW);
            LED_RED_ON(LOW);
            digitalWrite(PIN_LED_RXL, HIGH);
            digitalWrite(PIN_LED_TXL, HIGH);
            delay(100);
        };
  }    

    delay(2000);

}

