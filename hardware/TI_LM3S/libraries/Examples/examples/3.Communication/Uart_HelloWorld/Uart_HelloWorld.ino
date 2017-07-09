/* Hello World! arduino Serial.println it on uart1 demo by huawei<huaweiwx@sina.com> 2016.9.18 */

void setup() {
	Serial1.begin(115200);
}

void loop() {
  Serial1.println("Hello World!");
	delay(1000);
}
