/************************************************
�ļ���lm3s811_def.h
��;��Ԥ�õ�basic����
ver hw150804 for arduino
************************************************/

#ifndef __LM3S811_DEF_H__
  #define __LM3S811_DEF_H__
  
  #define portInputRegister(P)   portDATARegister(P)
  #define portOutputRegister(P)  portDATARegister(P)
  #define IO_REG_TYPE uint32_t

#define PINA   *portDATARegister(PA)
#define PINB   *portDATARegister(PB)
#define PINC   *portDATARegister(PC)
#define PIND   *portDATARegister(PD)
#define PINE   *portDATARegister(PE)

#define PORTA  *portDATARegister(PA)
#define PORTB  *portDATARegister(PB)
#define PORTC  *portDATARegister(PC)
#define PORTD  *portDATARegister(PD)
#define PORTE  *portDATARegister(PE)

  
//����ȱʡ��Ԥװ��ʾ����
#define PROG_DEFAULT811  \
"\"LM3S811Basic Test\"\n\
pm 13 1\n\
pm 48 1\n\
dw 13 1\n\
dw 48 0\n\
40 dt 13\n\
dt 48\n\
dms (1000)\n\
a=a+1\n\
print a\n\
goto 40\n\
end\n"


//out def   for SERx
#define USESERIAL
#if defined(USESERIAL1)
#define SERx Serial1
#else
#define SERx Serial
#endif

#define OUTDEV SERx

  #define PROG_RAM_DEFAULT PROG_DEFAULT811
  #define MAX_PROG_RAM 0xa00      //8k ram
  #define MAX_PROG_FLA 0x0fffUL  
  #define MAX_GOSUB_STACK_DEPTH   10
  #define MAX_FOR_STACK_DEPTH     10
  #define MAX_DO_STACK_DEPTH      10
//  #define MAX_PORTS_EVENTS      8
//  #define ADC_PORT              DDRF
//  #define NUM_COM_HANDLERS      2   //com0 events
//  #define NUM_PIN_HANDLERS      32  //pin 
  #define NUM_INTE_HANDLERS       8   //int0~7 events

//���ݲɼ��洢EEPROM���Ⱥ�λ��
//  #define EE2LENTH     0
//  #define EE1LENTH     0  
//  #define AT24CXX 512	   	// <---�ڴ��趨оƬ�ͺ�, 1����24C01; 16����24C16; 512����24C512

#endif    //!__BASICPROG_H__

