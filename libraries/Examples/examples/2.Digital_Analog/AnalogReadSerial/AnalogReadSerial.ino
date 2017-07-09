/*read ADC Analog pin ADC0 and print it on uart1 demo by huawei<huaweiwx@sina.com> 2016.9.18 */
void setup() {
  Serial1.begin(115200); 
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  ledFlash(10,990,1,1);  
  int sensorValue = analogRead(A0);
  Serial1.print("analog read A0 = ");
  Serial1.println(sensorValue, DEC);
}



