/*
 Shows How to use the SmartEverything RGB Led

 created 14 Jun 2015
 by Mik (smkk@amel-tech.com)
 */


  int i = 0;

void setup() {

  // LED & User Button are already initialized by the SME core.
  // it is not required to do here
  pinMode(PIN_LED_RXL, OUTPUT);
  pinMode(PIN_LED_TXL, OUTPUT);

}

void loop() {;

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
      i = 0;
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
}
