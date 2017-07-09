/************************************************
文件：basic_prog.h
用途：预置的basic程序
ver hw150804 for arduino
************************************************/

#ifndef __BASIC_PROG_H__
  #define __BASIC_PROG_H__

//开机缺省的预装演示代码
#define PROG_DEFAULT1  \
"10 print \"basic test1\"\n\
20 dt 13\n\
50 delay (1000)\n\
60 a=a+1\n\
70 print a\n\
80 goto 20\n\
100 end\n\0"

#define PROG_DEFAULT2  \
"10 on timer(0) = 5 gosub 200\n\
11 on timer(1) = 1  gosub 300\n\
12 on timer(2) = 20 gosub 400\n\
20 timer(0) on\n\
21 timer(1) on\n\
22 timer(2) on\n\
30 DDR(A,1)\n\
31 DDR(B,255)\n\
35 for i = 0 to 7\n\
40 setbit(B,i)\n\
50 delay(200)\n\
60 clrbit(B,i)\n\
80 next i\n\
90 goto 35\n\
200 print adc(1)\n\
210 iret\n\
300 togbit(A,0)\n\
310 iret\n\
400 timer(2) off\n\
401 timer(0) off\n\
410 iret\n\
800 end\n\0"

#define PROG_DEFAULT_ARM  \
"10 on timer(0) = 1 gosub 200\n\
11 on inte(0) = 3 gosub 400\n\
12 on inte(2) = 3 gosub 600\n\
13 on inte(8) = 3 gosub 800\n\
18 print \"ARM Demo beging...\"\n\
19 sys(81,0)\n\
20 timer(0) on\n\
21 inte(0) on\n\
22 inte(2) on\n\
23 inte(8) on\n\
30 a = 0\n\
35 a = a+1\n\
90 goto 35\n\
200 print \"adc(13)=\";adc(13)\n\
205 togbit(C,13)\n\
210 iret\n\
400 print# info(0),A\n\
410 iret\n\
600 print \"eerd(128)=\"; eerd(128)\n\
610 iret\n\
800 print \"SD TEST: OK\"\n\
820 iret\n\
900 end\n\0"

#define PROG_DEFAULT128  \
"10 on timer(0) = 10 gosub 200\n\
11 on timer(1)  = 1 gosub 300\n\
12 on inte(0)   = 3 gosub 400\n\
13 on inte(1)   = 3 gosub 600\n\
20 timer(0) on\n\
21 timer(1) on\n\
22 inte(0) on\n\
23 inte(1) on\n\
30 DDR(A,1)\n\
31 DDR(B,255)\n\
35 for i = 0 to 7\n\
40 setbit(B,i)\n\
50 delay(200)\n\
60 clrbit(B,i)\n\
80 next i\n\
90 goto 35\n\
200 print info(0),adc(1)\n\
210 iret\n\
300 togbit(A,0)\n\
310 iret\n\
400 print \"adc(0):\", adc(0)\n\
410 iret\n\
600 print \"pluse:\",info(80)\n\
610 iret\n\
800 end\n\0"

#define PROG_DEFAULT_M8  \
"10 a = 0\n\
20 for i = 0 to 100\n\
30 a = a+i\n\
40 next i\n\
50 print \"a=\";a\n\
800 end\n\0"

#define PROG_NULL \
"\0"

#endif  // !__BASICPROG_H__

