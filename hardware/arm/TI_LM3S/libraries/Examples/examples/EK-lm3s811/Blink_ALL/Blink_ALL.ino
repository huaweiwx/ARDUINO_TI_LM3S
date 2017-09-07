/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PD0, OUTPUT);
  pinMode(PD1, OUTPUT);
  pinMode(PE0, OUTPUT);
  pinMode(PE1, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PB0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PB0, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PB1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PB1, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PD0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PD0, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PD1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PD1, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PE0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PE0, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
  digitalWrite(PE1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);              // wait for a second
  digitalWrite(PE1, LOW);    // turn the LED off by making the voltage LOW
  delay(200);              // wait for a second
}
