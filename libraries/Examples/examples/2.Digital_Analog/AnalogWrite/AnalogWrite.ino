/*read PWM AnalogWrite port PA0 with occupy duty 50% demo by huawei<huaweiwx@sina.com> 2016.9.18 */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PA0, OUTPUT);
  analogWrite(PA0,125);  // occupy duty 50%
}

void loop() {
  ledFlash(10,990,1,1);  
}



