/*
 Shows How to use the SmartEverything RGB Led

 created 14 Jun 2015
 by Mik (smkk@amel-tech.com)
 */


  int i = 0;

void setup() {
  LED_GREEN_INIT;
  LED_BLUE_INIT;
  LED_RED_INIT;
  pinMode(PIN_LED_RXL, OUTPUT);
  pinMode(PIN_LED_TXL, OUTPUT);

}

void loop() {;

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
      i = 0;
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
}
