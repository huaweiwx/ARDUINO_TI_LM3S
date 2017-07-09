#include <LED.h>
//example for LED lib  LEDClass application   by huawei<huaweiwx@sina.com> 2016.10.28

//LEDClass Led;                  //init LEDClass Led(default LED_BUILTIN)  HIGH ON;
LEDClass Led(LED_BUILTIN, LOW);  //init LEDClass LeD(LED_BUILTIN, LOW)  LOW ON;

#if BOARD_NR_LED>1
  LEDClass Led1(LED_BUILTIN1, LOW);   //init LEDClass Led2  LOW ON;
#endif

#if BOARD_NR_LED>2
  LEDClass Led2(LED_BUILTIN2, LOW);   //init LEDClass Led3  LOW ON;
#endif


void setup() {
  //  put your setup code here, to run once:
  Led.Init();

#if BOARD_NR_LED>1
  Led1.Init();
#endif

#if BOARD_NR_LED>2
  Led2.Init();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

  Led.Flash(10, 990, 1);   

#if BOARD_NR_LED>1
  Led1.Flash(10, 490, 2);
#endif

#if BOARD_NR_LED>2
  Led2.Flash(10, 320, 3);
#endif
}
