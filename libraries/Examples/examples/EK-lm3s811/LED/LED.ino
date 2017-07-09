#include <LED.h>

LEDClass Led1;              //init LEDClass Led1(default LED_BUILTIN)  HIGH ON;
LEDClass Led2(LED2);   //init LEDClass Led2.LOW ON;
LEDClass Led3(LED3);   //init LEDClass Led3 LOW ON;
LEDClass Led4(LED4);   //init LEDClass Led2.LOW ON;
LEDClass Led5(LED5);   //init LEDClass Led3 LOW ON;
LEDClass Led6(LED6);   //init LEDClass Led2.LOW ON;
LEDClass Led7(LED7);   //init LEDClass Led3 LOW ON;
void setup() {
  Serial.begin(115200);
  //  put your setup code here, to run once:
  //  Serial.println(Led1.status,DEC);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("temp:"); Serial.println(getTemp(),DEC);
 Serial.print("ADC0:"); Serial.println(analogRead(A0),DEC);
 Serial.print("ADC1:"); Serial.println(analogRead(A1),DEC);
 Serial.print("ADC2:"); Serial.println(analogRead(A2),DEC);
 Serial.print("ADC3:"); Serial.println(analogRead(A3),DEC);
  Led1.Toggle();
  Led2.Flash(10, 490, 1);
  Led1.Toggle();
  Led3.Flash(10, 490, 2);
  Led1.Toggle();
  Led4.Flash(10, 490, 2);
  Led1.Toggle();
  Led5.Flash(10, 490, 2);
  Led1.Toggle();
  Led6.Flash(10, 490, 2);
  Led1.Toggle();
  Led7.Flash(10, 490, 2);
}
