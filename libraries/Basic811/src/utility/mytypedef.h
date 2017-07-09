/*------------------------------------------------*/
/* mytypedef.h  常量及基本类型宏定义              */
/* 通常由 myheads.h 调用                          */
/*      ver hw150804 for arduino                  */
/*------------------------------------------------*/

#ifndef   __MYTYPEDEF_H__
	#define   __MYTYPEDEF_H__

#define ARDUINO_BASIC_VAR 1508	
	
#ifndef BIT
  #define BIT(x) (1<<x)
#endif

#ifndef MIN
  #define MIN(a,b)			((a<b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b)			((a>b)?(a):(b))
#endif

#ifndef ABS
#define ABS(x)				((x>0)?(x):(-x))
#endif

#ifndef CONCAT
 #define CONCAT(a, b)            a ## b
// #define OUTPORT(name)     CONCAT(PORT, name)
// #define INPORT(name)      CONCAT(PIN, name)
// #define DDRPORT(name)     CONCAT(DDR, name)
#endif

#ifndef GETBIT
#define  GETBIT(x,y)       ((x) & (BIT(y)))
#endif

#ifndef SET_BITS
#define  SET_BITS(x,y)      ((x) |= (y))
#endif

#ifndef SETBIT
#define  SETBIT(x,y)       ((x) |= (BIT(y)))        
#endif

#ifndef CLR_BITS
#define  CLR_BITS(x,y)      ((x) &= ~(y))
#endif

#ifndef CLRBIT
#define  CLRBIT(x,y)       ((x) &= (~(BIT(y)))) 
#endif

#ifndef TOGBIT
#define  TOGBIT(x,y)       ((x)^=(BIT(y)))
#endif

// INPUT KEY
#define KEYDEF_CTRL_H    0x08
  #define KEYDEF_BS        0x08  //  \t
  #define KEYDEF_CTRL_I    0x09

//  \n  10
  #define KEYDEF_CTRL_J    0x0a  

//   \t  11
  #define KEYDEF_CTRL_K    0x0b

//   \f  12
  #define KEYDEF_CTRL_L    0x0c

//   \r  13
  #define KEYDEF_ENTER     0x0d
  #define KEYDEF_CTRL_M    0x0d

enum {
        KEYDEF_NULL   =   0x00,
	    KEYDEF_CTRL_A =   0x01,
			KEYDEF_CTRL_B =   0x02,
			KEYDEF_CTRL_C =   0x03,
			KEYDEF_CTRL_D =   0x04,
			KEYDEF_CTRL_E =   0x05,
			KEYDEF_CTRL_F =   0x06,
			KEYDEF_BEL    =   0x07,
	    KEYDEF_BACKSPACE  = 0x08,
			KEYDEF_TAB    = 0x09,
			KEYDEF_LF     = 0x0A,
      KEYDEF_VT     = 0x0b,
			KEYDEF_FF     = 0x0c, 
			KEYDEF_CR     = 0x0D,
			KEYDEF_CTRL_N = 0x0e,
			KEYDEF_CTRL_O = 0x0f,
      KEYDEF_CTRL_P = 0x10,
      KEYDEF_CTRL_Q = 0x11,
			KEYDEF_CTRL_R = 0x12,
			KEYDEF_CTRL_S = 0x13,
			KEYDEF_CTRL_T = 0x14,
			KEYDEF_CTRL_U = 0x15,
			KEYDEF_CTRL_V = 0x16,
			KEYDEF_CTRL_W = 0x17,
			KEYDEF_CTRL_X = 0x18,
			KEYDEF_CTRL_Y = 0x19,
			KEYDEF_CTRL_Z = 0x1a,
      KEYDEF_ESC    = 0x1B,
			KEYDEF_SPACE  = 0x20,
      KEYDEF_DEL    = 0x7F
};


	

	#ifdef   SYS_GLOBALS
		 #define  DEF_EXT
	#else
		 #define  DEF_EXT  extern
	#endif
	
//十进制数字转ASC
#ifndef DIG2ASC
	#define Dig2ASC(x)  (0X30+x)
#endif

//联合结构，用于参数传递
//MAX_ARGNUM  在config中定义,未定义缺省 32
#ifndef MAX_ARGNUM
  #define MAX_ARGNUM 32
#endif 
	
#define MAX_UARRYB  MAX_ARGNUM
#define MAX_UARRYW  MAX_UARRYB/2
#define MAX_UARRYL  MAX_UARRYW/2

#ifdef __cplusplus
extern "C"{ 
#endif 
typedef union{
	 uint32_t u32a[MAX_UARRYL];
	 uint16_t u16a[MAX_UARRYW];
	 uint8_t   u8a[MAX_UARRYB];
	 char      str[MAX_UARRYB];
	}ByteTypeU;
#ifdef __cplusplus
}//extern "C"
#endif 

#define ArgsTypeUserDef 5
#define ArgsTypestr     4
#define ArgsType32      3
#define ArgsType16      2
#define ArgsType8       1
#define ArgsTypeNone    0

#ifdef __cplusplus
extern "C"{ 
#endif 
typedef struct{
  uint8_t type;
  uint8_t counter;
  ByteTypeU uni;
}ArgsTypeDef;
#ifdef __cplusplus
} //extern "C"
#endif 

#endif
