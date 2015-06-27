/*
 Shows if the SME is running on Battery

 created 16 Jun 2015
 by Mik (smkk@amel-tech.com)
 */

#include <Arduino.h>

void setup() {
    // LED are already initialized by the SME core.
    // it is not required to do here

    SerialUSB.begin(115200);
}

void loop() {

    SerialUSB.print("SmartEverything is running on ");

    if (isOnBattery()) {
        ledGreenLight(40);
        ledRedLight(LOW);
        SerialUSB.println("Battery");
    } else {
        ledGreenLight(LOW);
        ledRedLight(40);
        SerialUSB.println("external power");
    }

    delay(2000);

}
