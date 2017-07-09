/*
  Blink2_Int.ino   内建LED闪亮功能使用实例
  Blink tested use internal function ledFlash(unit8_t ontime,uint8_t offtime,uint8_t again); 
  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin LED. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2016
  by huawei <huaweiwx@sina.com>
 */

// the setup function runs once when you press reset or power the board
void setup() {
   pinMode(LED_BUILTIN,OUTPUT);
}


// the loop function runs over and over again forever
void loop() {
    ledFlash(10,990,1,1); //led on(亮) 10ms  off(灭)990ms  again(循环) 1   
}
