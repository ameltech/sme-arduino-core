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

    // LED & User Button are already initialized by the SME core.
    // it is not required to do here
}

// the loop function runs over and over again forever
void loop() {

    if (isButtonOnePressed()) {
        ledBlueLight(HIGH);
    } else {
        ledBlueLight(LOW);
    }

    if (isButtonTwoPressed()) {
        ledRedLight(HIGH);
    } else {
        ledRedLight(LOW);
    }
}
