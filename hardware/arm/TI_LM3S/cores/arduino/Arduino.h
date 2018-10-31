#ifndef _ARDUINO_H_ 
#define _ARDUINO_H_ 

#include <stdint.h>
#include <stdbool.h> 
#include <string.h> 
#include <math.h>
#include "itoa.h"
#include "part.h"
#include <avr/dtostrf.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "binary.h" 
#include "inc/hw_types.h"  		
#include "inc/sysctl.h"
#include "inc/hw_nvic.h" 
#include "inc/gpio.h" 
#include "option.h"
#include "utils/utils_all.h"
#ifdef __cplusplus
extern "C"{ 
#endif 

//#define NOT_A_PORT   255
//#define NOT_A_PIN    255

#define NOT_ON_TIMER 0
#define NOT_ON_ADC 0x10

#define CHANGE  4
#define FALLING 3
#define RISING  2
#define HIGH  0x1
#define LOW   0x0


enum BitOrder {  /*compatible with arduino sam huaweiwx@sina.com 2018.1.12*/
	LSBFIRST = 0,
	MSBFIRST = 1
};

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3


#define SPI_LAST 0
#define SPI_CONTINUE 1

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5
#define PG 6
#define PH 7
#define PJ 8
#define PK 9
#define PL 10
#define PM 11
#define PN 12
#define PP 13
#define PQ 14
#define PR 15
#define PS 16
#define PT 17

#define TIMA 0
#define TIMB 8


#define T0A 0
#define T0B 1
#define T1A 2
#define T1B 3
#define T2A 4
#define T2B 5
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

typedef uint8_t boolean;
typedef uint8_t byte;

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() IntMasterEnable()
#define noInterrupts() IntMasterDisable()

#define clockCyclesPerMicrosecond() (F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))


typedef unsigned int word;


void init(void);
void setup(void);
void loop(void);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
unsigned long pulseIn(uint8_t ucPin, uint8_t state, unsigned long timeout);
#define pulseInLong pulseIn  /*compatible with arduino*/
void pinMode(uint8_t, uint8_t);
void digitalWriteHigh(uint8_t);
void digitalWriteLow(uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

void digitalToggle(uint8_t ucPin);  //add by huawei
extern void digitalFlash(uint8_t ucPin, uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on);//add by huawei
extern void ledFlash(uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on);//add by huawei
extern void errorFlash(uint8_t id);//add by huawei

uint16_t analogRead(uint8_t);
void analogWrite(uint8_t, int);
void analogReference(uint16_t);
void analogFrequency(uint32_t);
void analogResolution(uint16_t);
uint16_t getTemp(void);
void delay(uint32_t milliseconds);
/* void sleep(uint32_t milliseconds);
void sleepSeconds(uint32_t seconds);
void suspend(void);
extern volatile boolean stay_asleep;
#define wakeup() { stay_asleep = false; }
 */
void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

extern const uint8_t digital_pin_to_timer[];
extern const uint8_t digital_pin_to_port[];
extern const uint8_t digital_pin_to_bit_mask[];
extern const uint32_t timer_to_offset[];
extern const uint8_t timer_to_ab[];
extern const uint32_t timer_to_pin_config[];
extern const uint32_t port_to_base[];
extern const uint32_t digital_pin_to_analog_in[];

#define digitalPinToPort(P)       ( digital_pin_to_port[P] )
#define digitalPinToBitMask(P)    ( digital_pin_to_bit_mask[P] )
#define digitalPinToTimer(P)      ( digital_pin_to_timer[P] )
#define analogInPinToBit(P)       (P)                            //add by hw
#define timerToAB(P)              ( timer_to_ab[P] )
#define timerToOffset(P)          ( timer_to_offset[P] )
#define timerToPinConfig(P)       ( timer_to_pin_config[P] )
#define digitalPinToADCIn(P)      (((P)<NUM_ANALOG_INPUTS)? digital_pin_to_analog_in[P]:NOT_ON_ADC)
#define portBASERegister(P)       ((volatile uint32_t *) port_to_base[P])
#define portDATARegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x3FC ))
#define portDIRRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x400 ))
#define portIBERegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x408 ))
#define portIEVRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x40C ))
#define portIMRegister(P)         ((volatile uint32_t *)( port_to_base[P] + 0x410 ))
#define portRISRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x414 ))
#define portMISRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x418 ))
#define portICRRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x41C ))
#define portAFSELRegister(P)      ((volatile uint32_t *)( port_to_base[P] + 0x420 ))
#define portDR2RRegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x500 ))
#define portDR4RRegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x504 ))
#define portDR8RRegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x508 ))
#define portODRRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x50C ))
#define portPURRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x510 ))
#define portPDRRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x514 ))
#define portSLRRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x518 ))
#define portDENRegister(P)        ((volatile uint32_t *)( port_to_base[P] + 0x51C ))
/*#define portLOCKRegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x520 ))
#define portCRRegister(P)         ((volatile uint32_t *)( port_to_base[P] + 0x524 ))
#define portAMSELRegister(P)      ((volatile uint32_t *)( port_to_base[P] + 0x528 ))
#define portPCTLRegister(P)       ((volatile uint32_t *)( port_to_base[P] + 0x52C ))
#define portADCCTLRegister(P)     ((volatile uint32_t *)( port_to_base[P] + 0x530 ))
#define portMACTLRegister(P)      ((volatile uint32_t *)( port_to_base[P] + 0x534 ))
#define portPeriphID4Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFD0 ))
#define portPeriphID5Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFD4 ))
#define portPeriphID6Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFD8 ))
#define portPeriphID7Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFDC ))
#define portPeriphID0Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFE0 ))
#define portPeriphID1Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFE4 ))
#define portPeriphID2Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFE8 ))
#define portPeriphID3Register(P)  ((volatile uint32_t *)( port_to_base[P] + 0xFEC ))
#define portCellID0Register(P)    ((volatile uint32_t *)( port_to_base[P] + 0xFF0 ))
#define portCellID1Register(P)    ((volatile uint32_t *)( port_to_base[P] + 0xFF4 ))
#define portCellID2Register(P)    ((volatile uint32_t *)( port_to_base[P] + 0xFF8 ))
#define portCellID3Register(P)    ((volatile uint32_t *)( port_to_base[P] + 0xFFC ))
*/
// Implemented in wiring.c
void delayMicroseconds(unsigned int us);
unsigned long micros();
unsigned long millis();
void timerInit();
void registerSysTickCb(void (*userFunc)(uint32_t));
void yield(void);


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t ucPin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t ucPin, unsigned int frequency);
void tone(uint8_t ucPin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t ucPin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

/*C++ including option*/
#if USE_ARDUINOSTREAMING
#  include <Streaming.h>
#endif

#endif
#include "lm3s/pins_arduino.h"


#endif



