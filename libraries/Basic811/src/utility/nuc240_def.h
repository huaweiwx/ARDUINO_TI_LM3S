/************************************************
文件：nuc240_def.h
用途：预置的basic程序
ver hw150804 for arduino
************************************************/

#ifndef __NUC240_DEF_H__
  #define __NUC240_DEF_H__
//#include "arduino.h"  
//port IO
#define PINA  ((PA)->PIN)
#define PINB  ((PB)->PIN)
#define PINC  ((PC)->PIN)
#define PIND  ((PD)->PIN)
#define PINE  ((PE)->PIN)
#define PINF  ((PF)->PIN)
#define PORTA ((PA)->DOUT)
#define PORTB ((PB)->DOUT)
#define PORTC ((PC)->DOUT)
#define PORTD ((PD)->DOUT)
#define PORTE ((PE)->DOUT)
#define PORTF ((PF)->DOUT)

#define IO_REG_TYPE uint32_t

/*
  LED1(34), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED2(35), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED3(36), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED4(37), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED5(38), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED6(26), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED7(27), It work for NuEdu-NUC240, NuEdu-NANO130 and NuEdu-M451.
  LED8(29), It work for NuEdu-NUC240 and NuEdu-NANO130.
*/ 
  
//开机缺省的预装演示代码
#define PROG_DEFAULT240  \
"\Nuc240 Basic Test\"\n\
pm 34 1\n\
dt 34\n\
dms (1000)\n\
a=a+1\n\
\"a=\";a\n\
goto 40\n\
end\n"


  #define PROG_RAM_DEFAULT PROG_DEFAULT240
   
  #define MAX_PROG_RAM 0x1800        //16k ram
  #define MAX_PROG_FLA 0x0fffUL  
  #define MAX_GOSUB_STACK_DEPTH  10
  #define MAX_FOR_STACK_DEPTH    10
//  #define MAX_DO_STACK_DEPTH   10
//  #define MAX_PORTS_EVENTS     8
//  #define ADC_PORT             DDRF
//  #define NUM_COM_HANDLERS     2   //com0   events
//  #define NUM_PIN_HANDLERS     32  //pin 
  #define NUM_INTE_HANDLERS      8   //int0~7 events
//  #define AUTO_RUN_IN          PINF
//  #define AUTO_RUN_SET         7
//  #define AUTO_RUN_EEP         6
//  #define AUTO_RUN_COM         5
//  #define LED_DDR              DDRA
//  #define LED_PORT             PORTA 
//  #define LED0_BIT             0
//数据采集存储EEPROM长度和位置
  #define AT24CXX 512	   	   // <---在此设定芯片型号, 1代表24C01; 16代表24C16; 512代表24C512
  #define EE1LENTH  0x10000    //外部

#ifdef AT24CXX                 //have extern i2c eeprom
   #define EE_DATA_RESEVE       8 
   #define EE_DATA_USER         100                            //  user data 
   #define EE_DATA_LENTH       (EE_DATA_RESEVE+EE_DATA_USER)   //
   #define EE_DATA_USER_START   EE_DATA_RESEVE*2
   #define EE_PROG_START_ADR    EE_DATA_LENTH*2
   #define EE_PROG_END_ADR      EE1LENTH-1
 
//#include "Wire.h"
#include "EE1PROM.h"
   
#else                                                         //  have not extern i2ceeprom  
   #warning "have extern ee2prom at24cxx" 
#endif   // end AT24CXX

  #define NUM_DIGITIAL_PINS 100
  #define NUM_ANALOG_PINS   8
#endif  // !__BASICPROG_H__

 