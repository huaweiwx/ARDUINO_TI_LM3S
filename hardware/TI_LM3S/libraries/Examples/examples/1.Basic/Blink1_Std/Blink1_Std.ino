/*
  Blink_Std.ino
  Turns on an LED on for 0.5 second, then off for 0.5 second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 8 May 2016
  by huawei <huaweiwx@sina.com>
 */


// the setup function runs once when you press reset or power the board
int LED = LED_BUILTIN;
void setup() {
  // initialize digital LED as an output.
  pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for 500ms
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);              // wait for 500ms
}
