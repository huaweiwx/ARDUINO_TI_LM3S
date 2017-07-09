#include <LED.h>

LEDClass Led1;              //init LEDClass Led1(default LED_BUILTIN)  HIGH ON;
LEDClass Led2(LED2, LOW);   //init LEDClass Led2.LOW ON;
LEDClass Led3(LED3, LOW);   //init LEDClass Led3 LOW ON;

DISP  disp;       //init DISPClass disp;
void setup() {
  disp.home();
  disp.print("    HELLO disp!\n"); // *str,x,y,lever
  disp.print("012345678901234567890\n"); // *str,x,y,lever
  disp.print("   abcdefghijklmn\n\n"); // *str,x,y,lever
  disp.print("new line No = "); // *str,x,y,lever
  disp.print(5,DEC);
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint8_t i;
  uint16_t temp;
  Led1.Flash(10, 990, 1);
  Led2.Flash(10, 490, 2);
  Led3.Flash(10, 320, 3);
  //  digitalToggle(LED1);
  //  digitalToggle(LED2);
  //  digitalToggle(LED3);
   disp.setCursor(7,0);  disp.print("                     ");//clear this line
   disp.setCursor(7,0);  disp.print(analogRead(A0),DEC);
   disp.setCursor(7,5);  disp.print(analogRead(A1),DEC);
   disp.setCursor(7,11); disp.print(analogRead(A2),DEC);
   disp.setCursor(7,16); disp.print(analogRead(A3),DEC);
   disp.setCursor(8,0);  disp.print("chip tmp: ");
   temp = getTemp();
   disp.print(temp/100,DEC);
   temp %=100;
   if (temp >9) disp.print(".");
   else disp.print(".0");
   disp.print(temp,DEC);
}
