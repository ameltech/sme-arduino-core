/*
 Shows if the SME is running on Battery

 created 16 Jun 2015
 by Mik (smkk@amel-tech.com)
 */

#include <Arduino.h>

void setup() {
    // LED is already initialized by the SME core.
    // it is not required to do here
}

void loop() {

    setStepUp(LOW);
    ledBlueLight(LOW);

    delay(500);

    setStepUp(HIGH);
    ledBlueLight(40);
    delay(200);

}
