/************************************************
�ļ���basic.h
��;��BASIC configϵͳ����
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
//�ؼ�����󳤶�+ \0    �� return\0 = 7
#define MAX_TOKENLEN  7
//���͸�set info sys ���(u8)��������
#define MAX_ARGNUM   32
//������ 26�� �̶�������һ����ǰ��ʱ������
#define MAX_VARNUM 27 
//����ͨ����ʱ���� ANS  //�൱�ڼ������ϵ�ans Ӧ����ʹ�ã���
#define ANS_VARNUM MAX_VARNUM-1
//�������λ��+1  ����Ƶ fcpu  160,000,000 �� 160M
#define MAX_NUMLEN 11  
//hex�������λ��+2  ��0x0000ffff
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
#define IN_EVENTS_PROC   3  //���¼�������
#define AUTO_RUN         4
#define PAUSE_EVENT      5
//#define REV            6-7  //����

//for status:  sGLOBEL_T.Ctrl
#define OUT_LCD        15  //��ʾRTCʱ�� for stm32
#define SHOW_ECHO        14  //������Կ���
//#define PRINT_HEXMODE    13
#define REV12            12  //����
#define REV11            11  //����
#define REV10            10  //����
#define REV9             9  //����
#define REV8             8  //����
#define LED_BLINK        7  //  LED ����
#define REV6             6  //����
#define REV5             5  //����
#define REV4             4  //����
#define REV3             3  //����
#define REV2             2  //����
#define REV1             1  //����
#define REV0             0  //����

#ifdef __cplusplus
extern "C"{ 
#endif 

//----------------------------------------------------------------------
//basic ȫ�ֱ����ṹ
typedef struct  {
  volatile uint16_t  Flags;            //��־��mask
  volatile uint16_t  Ctrls;            //����λ;         
  volatile uint8_t   FlagsMask;        //mask
  uint8_t    print_base;                //�����������;
  uint8_t    Err_id;           //���д����
  uint16_t   Err_pl;           //���д���λ��
  uint16_t   pause_adr;        //��ͣ��ַ
  uint8_t    CurrentToken;     //��ǰ�ؼ���
  uint8_t    CurrentFuncToken; //��ǰ����
  char *pBas;            //basic��ʼ��ַָ��
  char *pChar;           //basic��ǰ��ַָ�� 
  char *pNext;           //basic�¸��������ַָ��
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
//  uint8_t  TimerNo;      // counter no ��ʱ����
  uint8_t  timernum;     //canno  ͨ����
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
