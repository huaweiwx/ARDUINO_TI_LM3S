#include <LED.h>

LEDClass Led1;              //init LEDClass Led1(default LED_BUILTIN)  HIGH ON;
LEDClass Led2(LED2,LOW);   //init LEDClass Led2.LOW ON;
LEDClass Led3(LED3,LOW);   //init LEDClass Led3 LOW ON;
DISP disp;
void setup() {
  Serial.begin(115200);
  //  put your setup code here, to run once:
  //  Serial.println(Led1.status,DEC);
  disp.setCursor(4,15); //x,y,lever
  disp.print("HELLO disp!"); // *str
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("test!");
  Led1.Flash(10, 990, 1);
  Led2.Flash(10, 490, 2);
  Led3.Flash(10, 320, 3);
}
