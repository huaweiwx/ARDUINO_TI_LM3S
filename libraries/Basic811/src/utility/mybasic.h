/************************************************
文件：basic.h
用途：BASIC config系统配置
//ver hw150804 for arduino
************************************************/
#ifndef __BASIC_H__
  #define __BASIC_H__

//----------------------------------------------------------------------
//for EVENTS
#define NUM_TIMER_HANDLERS      8   //time0~7 events

//bit
#define EVENT_COM0           0
#define EVENT_COM1           1
#define EVENT_TIMER          2
#define EVENT_INTE           3    //int0//int1
#define EVENT_PIN            4    
#define EVENT_IIC            5
#define EVENT_SPI            6
#define EVENT_ERR            7


//for basic
//关键词最大长度+ \0    如 return\0 = 7
#define MAX_TOKENLEN  7
//传送给set info sys 最大(u8)参数个数
#define MAX_ARGNUM   32
//变量数 26个 固定变量，一个当前临时变量；
#define MAX_VARNUM 27 
//！！通用临时变量 ANS  //相当于计算器上的ans 应谨慎使用！！
#define ANS_VARNUM MAX_VARNUM-1
//数字最大位数+1  如主频 fcpu  160,000,000 ＝ 160M
#define MAX_NUMLEN 11  
//hex数字最大位数+2  如0x0000ffff
#define MAX_HEXNUMLEN 10  

//for mcus
#if  defined(__AVR_ATmega16__)
 #include "atmega16_def.h"
#elif defined(__AVR_ATmega32__)
  #include "atmega32_def.h"
#elif  defined(__AVR_ATmega328P__)
 #include "atmega328_def.h"
#elif defined(__AVR_ATmega64__)||defined(__AVR_ATmega128__)
 #include "atmega64_128_def.h"
#elif defined(ARDUINO_LM3S811EVB)||defined(ARDUINO_LM3S811ND)
  #include "lm3s811_def.h" 
#elif defined(__M0516__)
  #include "m0516_def.h"
#elif defined(__NUC240__)
  #include "nuc240_def.h"
#elif defined(ARDUINO_GENERIC_STM32F103C)||defined(ARDUINO_GENERIC_STM32F103V)||defined(ARDUINO_GENERIC_STM32F103Z)
  #include "stm32f103x_def.h"  
#elif defined(__LM4F232H5QD__)
  #include "lm4f232_def.h"
#else 
  #error "not define basic board in mybasic.h"	
#endif


//for status:  sGLOBEL_T.Flags
#define GLOBLES_NEW_LINE 0
#define IS_LINE_NUM      1
#define GLOBLES_END      2
#define IN_EVENTS_PROC   3  //在事件处理中
#define AUTO_RUN         4
#define PAUSE_EVENT      5
//#define REV            6-7  //保留

//for status:  sGLOBEL_T.Ctrl
#define OUT_LCD        15  //显示RTC时间 for stm32
#define SHOW_ECHO        14  //输入回显控制
//#define PRINT_HEXMODE    13
#define REV12            12  //保留
#define REV11            11  //保留
#define REV10            10  //保留
#define REV9             9  //保留
#define REV8             8  //保留
#define LED_BLINK        7  //  LED 闪亮
#define REV6             6  //保留
#define REV5             5  //保留
#define REV4             4  //保留
#define REV3             3  //保留
#define REV2             2  //保留
#define REV1             1  //保留
#define REV0             0  //保留

#ifdef __cplusplus
extern "C"{ 
#endif 

//----------------------------------------------------------------------
//basic 全局变量结构
typedef struct  {
  volatile uint16_t  Flags;            //标志及mask
  volatile uint16_t  Ctrls;            //控制位;         
  volatile uint8_t   FlagsMask;        //mask
  uint8_t    print_base;                //数字输出进制;
  uint8_t    Err_id;           //运行错误号
  uint16_t   Err_pl;           //运行错误位置
  uint16_t   pause_adr;        //暂停地址
  uint8_t    CurrentToken;     //当前关键词
  uint8_t    CurrentFuncToken; //当前函数
  char *pBas;            //basic起始地址指针
  char *pChar;           //basic当前地址指针 
  char *pNext;           //basic下个待处理地址指针
  char *pRam;
  uint16_t lenRam;
} sGLOBEL_T;

typedef struct  {
   volatile uint16_t   flags;                // bitmapped for each type of event that is enabled
   volatile uint16_t   masks;
   char  *save_ptr;
//   uint8_t save_token;
//   uint16_t save_linenum;
}EVENTS_StackTypeDef;


typedef struct {
   volatile uint16_t   flags;                // bitmapped for each type of event that is enabled
   volatile uint16_t   masks;
//   uint16_t  val[NUM_INTE_HANDLERS];
   uint16_t gosub_line [NUM_INTE_HANDLERS];    // handler for inte()
}EVENTS_ExtiStackTypeDef;


typedef struct{
//  uint8_t  TimerNo;      // counter no 定时器号
  uint8_t  timernum;     //canno  通道号
  volatile uint16_t  flags;        // bitmapped for 8 timer events
  volatile uint16_t  masks;
  uint16_t gosub_line [NUM_TIMER_HANDLERS];    // handler for TIMER()
  uint16_t clock      [NUM_TIMER_HANDLERS];
  uint16_t interval   [NUM_TIMER_HANDLERS];
  uint16_t tick;
//  s8		   ptr;
} EVENTS_TimeStackTypeDef;

#ifdef __cplusplus
} //extern "C"
#endif 

//----------------------------------------------------------------------
#endif  // !__CONFIG_H__
#if defined(__AVR__)|defined(__arm__)  //for AVR arm Seriers
#else
#error "for AVR & ARM only!"
#endif
