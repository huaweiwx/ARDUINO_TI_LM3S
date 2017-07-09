#include <LED.h>

LEDClass Led1;              //init LEDClass Led1(default LED_BUILTIN)  HIGH ON;
LEDClass Led2(LED2, LOW);   //init LEDClass Led2.LOW ON;
LEDClass Led3(LED3, LOW);   //init LEDClass Led3 LOW ON;

DISP  disp;       //init DISPClass disp;
void setup() {
  disp.StringDraw("HELLO disp!  1"); // *str,x,y,lever
  disp.StringDraw("HELLO disp!  3", 4, 13, 3); // *str,x,y,lever
  disp.StringDraw("HELLO disp!  5", 4, 23, 5); // *str,x,y,lever
  disp.StringDraw("HELLO disp!  7", 4, 33, 7); // *str,x,y,lever
  disp.StringDraw("HELLO disp!  9", 4, 43, 9); // *str,x,y,lever
  disp.StringDraw("HELLO disp! 11", 4, 53, 11); // *str,x,y,lever
  disp.StringDraw("HELLO disp! 13", 4, 63, 13); // *str,x,y,lever
  disp.StringDraw("HELLO disp! 15", 4, 73, 15); // *str,x,y,lever
  disp.StringDraw("     OK   !"   , 4, 83, 15); // *str,x,y,lever
}

void loop() {
  // put your main code here, to run repeatedly:
  Led1.Flash(10, 990, 1);
  Led2.Flash(10, 490, 2);
  Led3.Flash(10, 320, 3);
  //  digitalToggle(LED1);
  //  digitalToggle(LED2);
  //  digitalToggle(LED3);
}
