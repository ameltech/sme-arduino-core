/*
 Shows The PWM functionalities on the RGB Led

 created 18 Jun 2015

 by Mik (smkk@amel-tech.com)
 */

#include <Arduino.h>

long randNumberR, randNumberG, randNumberB;

void setup(){
    randomSeed(analogRead(0));
}

void loop(){
    randNumberR = random(128);
    randNumberG = random(128);
    randNumberB = random(128);



  ledGreenLight(randNumberG);
  ledBlueLight(randNumberB);
  ledRedLight(randNumberR);

    delay(500);
}