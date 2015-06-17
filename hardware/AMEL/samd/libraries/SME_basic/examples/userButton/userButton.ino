/*
* userButton.ino
*
* Example to sohws how the 2 userbutton works on Smarteverything
*
* Created: 6/17/2015 10:32:11 PM
*
*  Author: Mik
*
*/

#include <Arduino.h>


// the setup function runs once when you press reset or power the board
void setup() {
        
    // LED & User Button are already initilized by the SME core.
    // it is not required to do here

    SerialUSB.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

    if (button1IsPressed()) {
        LED_BLUE_ON(HIGH);
    } else {
        LED_BLUE_ON(LOW);
    }
    
    if (button2IsPressed()) {
        LED_RED_ON(HIGH);
    } else {
        LED_RED_ON(LOW);
    }
}
