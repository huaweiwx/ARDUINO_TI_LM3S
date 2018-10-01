#ifndef _PINS_ENERGIA_H
#define _PINS_ENERGIA_H

#ifndef BV
#define BV(x) (1 << (x))
#endif

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/adc.h"
#include "variant.h"

#if defined(PART_LM3S811)
#define CHIP_PIN_LIST \
  PIN(A,0),PIN(A,1),PIN(A,2),PIN(A,3),PIN(A,4),PIN(A,5), \
  PIN(B,0),PIN(B,1),PIN(B,2),PIN(B,3),PIN(B,4),PIN(B,5),PIN(B,6),PIN(B,7), \
  PIN(C,4),PIN(C,5),PIN(C,6),PIN(C,7), \
  PIN(D,0),PIN(D,1),PIN(D,2),PIN(D,3),PIN(D,4),PIN(D,5),PIN(D,6),PIN(D,7), \
  PIN(E,0),PIN(E,1),



#elif defined(PART_LM3S8962)
#define CHIP_PIN_LIST \
  PIN(A,0),PIN(A,1),PIN(A,2),PIN(A,3),PIN(A,4),PIN(A,5),PIN(A,6),PIN(A,7),\
  PIN(B,0),PIN(B,1),PIN(B,2),PIN(B,3),PIN(B,4),PIN(B,5),PIN(B,6),         \
  PIN(C,4),PIN(C,5),PIN(C,6),PIN(C,7), \
  PIN(D,0),PIN(D,1),PIN(D,2),PIN(D,3),PIN(D,4),PIN(D,5),PIN(D,6),PIN(D,7), \
  PIN(E,0),PIN(E,1),PIN(E,2),PIN(E,3),\
  PIN(F,0),PIN(F,1),PIN(F,2),PIN(F,3),\
  PIN(G,0),PIN(G,1),

#else
#error "PART undef! please add me"
#endif

#ifdef VARIANT_PIN_LIST
#define PIN_LIST VARIANT_PIN_LIST
#else
#define PIN_LIST CHIP_PIN_LIST
#endif

#if defined(ARDUINO_MAIN) && !(defined(VARIANT_PIN_LIST))
#if defined(PART_LM3S811)
const uint32_t port_to_base[] = {
  (uint32_t) GPIO_PORTA_BASE,
  (uint32_t) GPIO_PORTB_BASE,
  (uint32_t) GPIO_PORTC_BASE,
  (uint32_t) GPIO_PORTD_BASE,
  (uint32_t) GPIO_PORTE_BASE,
};

const uint8_t digital_pin_to_timer[] = {
  NOT_ON_TIMER,   //17  PA0/U0RX
  NOT_ON_TIMER,   //18  PA1/U0TX
  NOT_ON_TIMER,   //19  PA2/SSICLK
  NOT_ON_TIMER,   //20  PA3//SSIFss
  NOT_ON_TIMER,   //21  PA4//SSIRX
  NOT_ON_TIMER,   //22  PA5//SSITX

  NOT_ON_TIMER,   //29  PB0/PWM2
  NOT_ON_TIMER,   //30  PB1/PWM1
  NOT_ON_TIMER,   //33  PB2/I2CSCL
  NOT_ON_TIMER,   //34  PB3/I2CSDA
  NOT_ON_TIMER,   //44  PB4/C0-
  T2B,      //43  PB5
  NOT_ON_TIMER,   //42  PB6/C0+
  NOT_ON_TIMER,   //41  PB7/TRST

  //  NOT_ON_TIMER,   //40  PC0/TCK/SWCLK
  //  NOT_ON_TIMER,   //39  PC1/TMS/SWDIO
  //  NOT_ON_TIMER,   //38  PC2/TDI
  //  NOT_ON_TIMER,   //37  PC3/TDO/SWO
  NOT_ON_TIMER,   //14  PC4
  T0B,      //13  PC5
  T1B,      //12  PC6
  T2A,      //11  PC7/CCP4

  NOT_ON_TIMER,   //25  PD0/PWM0
  NOT_ON_TIMER,   //26  PD1/PWM1
  NOT_ON_TIMER,   //27  PD2/U1RX
  NOT_ON_TIMER,   //28  PD3/U1TX
  T0A,      //45  PD4
  T1A,      //46  PD5
  NOT_ON_TIMER,   //47  PD6/FAULT
  NOT_ON_TIMER,   //48  PD7/C0o

  NOT_ON_TIMER,   //35  PE0/PWM4
  NOT_ON_TIMER,   //36  PE1/PWM5
};

const uint8_t digital_pin_to_port[] = {
  PA,       //17 PA0/U0RX
  PA,       //18 PA1/U0TX
  PA,       //19 PA2/SSICLK
  PA,         //20 PA3/SSIFss
  PA,       //21 PA4/SSIRX
  PA,       //22 PA5/SSITX

  PB,       //29 PB0/PWM2
  PB,       //30 PB1/PWM1
  PB,         //33 PB2/I2CSCL
  PB,         //34 PB3/I2CSDA
  PB,         //44  PB4/C0-
  PB,         //43  PB5/CCP5
  PB,         //42  PB6/C0+
  PB,         //41  PB7/TRST

  //  PC,     //40  PC0/TCK/SWCLK
  //  PC,     //39  PC1/TMS/SWDIO
  //  PC,     //38  PC2/TDI
  //  PC,     //37  PC3/TDO/SWO
  PC,     //14  PC4
  PC,     //13;   //PC5/CCP1
  PC,     //12  PC6/CCP3
  PC,     //11  PC7/CCP4

  PD,     //25  PD0/PWM0
  PD,     //26  PD1/PWM1
  PD,     //27  PD2/U1RX
  PD,     //28  PD3/U1TX
  PD,     //45  PD4/CCP0
  PD,     //46  PD5/CCP2
  PD,     //47  PD6/FAULT
  PD,     //48  PD7/C0o

  PE,     //35  PE0/PWM4
  PE,     //36  PE1/PWM5
};
const uint8_t digital_pin_to_bit_mask[] = {
  BV(0),      //17  PA0/U0RX
  BV(1),      //18  PA1/U0TX
  BV(2),      //19  PA2/SSICLK
  BV(3),      //20  PA3//SSIFss
  BV(4),      //21   PA4//SSIRX
  BV(5),      //22  PA5//SSITX

  BV(0),      //29  PB0/PWM2
  BV(1),      //30  PB1/PWM1
  BV(2),      //33  PB2/I2CSCL
  BV(3),      //34  PB3/I2CSDA
  BV(4),      //44  PB4/C0-
  BV(5),      //43  PB5/CCP5
  BV(6),      //42  PB6/C0+
  BV(7),    //41  PB7/TRST

  //  BV(0),    //40  PC0/TCK/SWCLK
  //  BV(1),    //39  PC1/TMS/SWDIO
  //  BV(2),    //38  PC2/TDI
  //  BV(3),    //37  PC3/TDO/SWO
  BV(4),      //14  PC4
  BV(5),      //13  PC5/CCP1
  BV(6),      //12  PC6/CCP3
  BV(7),      //11  PC7/CCP4

  BV(0),      //25  PD0/PWM0
  BV(1),      //26  PD1/PWM1
  BV(2),      //27  PD2/U1RX
  BV(3),      //28  PD3/U1TX
  BV(4),      //45  PD4/CCP0
  BV(5),      //46  PD5/CCP2
  BV(6),      //47  PD6/FAULT
  BV(7),      //48  PD7/C0o

  BV(0),      //35  PE0/PWM4
  BV(1),      //36  PE1/PWM5
};

const uint32_t timer_to_offset[] = {
  T0A,
  T0B,
  T1A,
  T1B,
  T2A,
  T2B,
};

const uint8_t timer_to_ab[] = {
  TIMA,
  TIMB,
  TIMA,
  TIMB,
  TIMA,
  TIMB,
};
const uint32_t timer_to_pin_config[] = {
  CCP0_PORT,
  CCP1_PORT,
  CCP2_PORT,
  CCP3_PORT,
  CCP4_PORT,
  CCP5_PORT,
};
const uint32_t digital_pin_to_analog_in[] = {
  ADC_CTL_TS,     //  0 - Temp sensor
  ADC_CTL_CH0,    //  1 - ADC0
  ADC_CTL_CH1,  //  2 - ADC1
  ADC_CTL_CH2,    //  3 - ADC2
  ADC_CTL_CH3,  //  4 - ADC3
};

#elif defined(PART_LM3S8962)

const uint32_t port_to_base[] = {
  (uint32_t) GPIO_PORTA_BASE,
  (uint32_t) GPIO_PORTB_BASE,
  (uint32_t) GPIO_PORTC_BASE,
  (uint32_t) GPIO_PORTD_BASE,
  (uint32_t) GPIO_PORTE_BASE,
  (uint32_t) GPIO_PORTF_BASE,
  (uint32_t) GPIO_PORTG_BASE,
};

const uint8_t digital_pin_to_timer[] = {
  NOT_ON_TIMER,   //26 15 PA0/U0RX
  NOT_ON_TIMER,   //27 16 PA1/U0TX
  NOT_ON_TIMER,   //28 17 PA2/SSICLK
  NOT_ON_TIMER,   //29 18 PA3//SSIFss
  NOT_ON_TIMER,   //30 19 PA4//SSIRX
  NOT_ON_TIMER,   //31 20 PA5//SSITX
  NOT_ON_TIMER,   //34  PA6/CCP1
  NOT_ON_TIMER,   //35  PA7

  NOT_ON_TIMER,   //66  PB0/PWM2
  NOT_ON_TIMER,   //67  PB1/PWM1
  NOT_ON_TIMER,   //70  PB2/I2CSCL
  NOT_ON_TIMER,   //71  PB3/I2CSDA
  NOT_ON_TIMER,   //92  PB4/C0-
  T2B,      //91  PB5/C0o
  NOT_ON_TIMER,   //90  PB6/C0+
  NOT_ON_TIMER,   //89  PB7/TRST

  NOT_ON_TIMER,   //80  PC0/TCK/SWCLK
  NOT_ON_TIMER,   //79  PC1/TMS/SWDIO
  NOT_ON_TIMER,   //78  PC2/TDI
  NOT_ON_TIMER,   //77  PC3/TDO/SWO
  NOT_ON_TIMER,   //25 14 PC4/PhA0
  T0B,      //24 13 PC5
  T1B,      //23 12 PC6/PhB0
  NOT_ON_TIMER, //22 11 PC7/

  NOT_ON_TIMER,   //10 0 PD0/CAN0Rx
  NOT_ON_TIMER,   //11 1 PD1/CAN0Tx
  NOT_ON_TIMER,   //12 2 PD2/U1RX
  NOT_ON_TIMER,   //13 3 PD3/U1TX
  T0A,      //95 PD4/CCP0
  T1A,      //96  PD5
  NOT_ON_TIMER,   //99  PD6/FAULT
  NOT_ON_TIMER,   //100  PD7/IDXo

  NOT_ON_TIMER,   //72  PE0/PWM4
  NOT_ON_TIMER,   //73  PE1/PWM5
  NOT_ON_TIMER,   //74  PE2/PhB1
  NOT_ON_TIMER,   //75  PE3/PhA1


  NOT_ON_TIMER,   //47  PF0/PWM0
  NOT_ON_TIMER,   //61  PF1/PWM0
  NOT_ON_TIMER,   //60  PF2/PWM0
  NOT_ON_TIMER,   //59  PF3/PWM0

  NOT_ON_TIMER,   //19 5 PG0
  NOT_ON_TIMER,   //18 4 PG1/PWM1
};

const uint8_t digital_pin_to_port[] = {
  PA,   //26 15 PA0/U0RX
  PA,   //27 16 PA1/U0TX
  PA,   //28 17 PA2/SSICLK
  PA,   //29 18 PA3//SSIFss
  PA,   //30 19 PA4//SSIRX
  PA,   //31 20 PA5//SSITX
  PA,   //34 21 PA6/CCP1
  PA,   //35 22 PA7

  PB,   //66 27 PB0/PWM2
  PB,   //67 28 PB1/PWM1
  PB,   //70 29 PB2/I2CSCL
  PB,   //71 30 PB3/I2CSDA
  PB,   //92 42 PB4/C0-
  PB,   //91 41 PB5/C0o
  PB,   //90 40 PB6/C0+
  PB,   //89 39 PB7/TRST

  PC,   //80 38 PC0/TCK/SWCLK
  PC,   //79 37 PC1/TMS/SWDIO
  PC,   //78 36 PC2/TDI
  PC,   //77 35 PC3/TDO/SWO
  PC,   //25 14 PC4/PhA0
  PC,   //24 13 PC5
  PC,   //23 12 PC6/PhB0
  PC,   //22 11 PC7/

  PD,   //10 5 PD0/CAN0Rx
  PD,   //11 6 PD1/CAN0Tx
  PD,   //12 7 PD2/U1RX
  PD,   //13 8 PD3/U1TX
  PD,   //95 43 PD4/CCP0
  PD,   //96 44 PD5
  PD,   //99 45 PD6/FAULT
  PD,   //100 46 PD7/IDXo

  PE,   //72 31 PE0/PWM4
  PE,   //73 32 PE1/PWM5
  PE,   //74 33 PE2/PhB1
  PE,   //75 34 PE3/PhA1

  PF,   //47 23 PF0/PWM0
  PF,   //61 26 PF1/PWM0
  PF,   //60 25 PF2/PWM0
  PF,   //59 24 PF3/PWM0

  PG,   //19 10 PG0
  PG,   //18 9 PG1/PWM1
};

const uint8_t digital_pin_to_bit_mask[] = {
  BV(0),   //26 15 PA0/U0RX
  BV(1),   //27 16 PA1/U0TX
  BV(2),   //28 17 PA2/SSICLK
  BV(3),   //29 18 PA3//SSIFss
  BV(4),   //30 19 PA4//SSIRX
  BV(5),   //31 20 PA5//SSITX
  BV(6),   //34 21 PA6/CCP1
  BV(7),   //35 22 PA7

  BV(0),   //66 27 PB0/PWM2
  BV(1),   //67 28 PB1/PWM1
  BV(2),   //70 29 PB2/I2CSCL
  BV(3),   //71 30 PB3/I2CSDA
  BV(4),   //92 42 PB4/C0-
  BV(5),   //91 41 PB5/C0o
  BV(6),   //90 40 PB6/C0+
  BV(7),   //89 39 PB7/TRST

  BV(0),   //80 38 PC0/TCK/SWCLK
  BV(1),   //79 37 PC1/TMS/SWDIO
  BV(2),   //78 36 PC2/TDI
  BV(3),   //77 35 PC3/TDO/SWO
  BV(4),   //25 14 PC4/PhA0
  BV(5),    //24 13 PC5
  BV(6),    //23 12 PC6/PhB0
  BV(7),    //22 11 PC7/

  BV(0),   //10 5 PD0/CAN0Rx
  BV(1),   //11 6 PD1/CAN0Tx
  BV(2),   //12 7 PD2/U1RX
  BV(3),   //13 8 PD3/U1TX
  BV(4),    //95 43 PD4/CCP0
  BV(5),    //96 44 PD5
  BV(6),   //99 45 PD6/FAULT
  BV(7),   //100 46 PD7/IDXo

  BV(0),   //72 31 PE0/PWM4
  BV(1),   //73 32 PE1/PWM5
  BV(2),   //74 33 PE2/PhB1
  BV(3),   //75 34 PE3/PhA1


  BV(0),   //47 23 PF0/PWM0
  BV(1),   //61 26 PF1/PWM0
  BV(2),   //60 25 PF2/PWM0
  BV(3),   //59 24 PF3/PWM0


  BV(0),   //19 10 PG0
  BV(1),   //18 9  PG1/PWM1
};

const uint32_t timer_to_offset[] = {
  T0A,
  T0B,
  T1A,
  T1B,
  T2A,
  T2B,
};

const uint8_t timer_to_ab[] = {
  TIMA,
  TIMB,
  TIMA,
  TIMB,
  TIMA,
  TIMB,
};

const uint32_t timer_to_pin_config[] = {
  /*    CCP0_PORT,
      CCP1_PORT,
      CCP2_PORT,
      CCP3_PORT,
      CCP4_PORT,
      CCP5_PORT,
  */
};
const uint32_t digital_pin_to_analog_in[] = {
  ADC_CTL_TS,     //  0 - Temp sensor
  ADC_CTL_CH0,    //  1 - ADC0
  ADC_CTL_CH1,  //  2 - ADC1
  ADC_CTL_CH2,    //  3 - ADC2
  ADC_CTL_CH3,  //  4 - ADC3
};
#else
#error "undef"
#endif

#endif


#ifdef __cplusplus

class __ConstPin {
  public:
    constexpr __ConstPin(const uint32_t port, const  uint32_t pin, const int val)
      : port(port), pin(pin), val(val) {};
    constexpr operator int() const {
      return val;
    }
    const uint32_t port;
    const  uint32_t pin;
    const int val;
};
#define PIN(a, b) __P##a##b
enum {
  PIN_LIST
  NUM_DIGITAL_PINS,
};
#undef PIN

#define PIN(a, b)  P##a##b((const uint32_t)GPIO_PORT ## a ##_BASE, BV(b), __P##a##b)
constexpr __ConstPin PIN_LIST __IGNORE(0, 0, -1);
#undef PIN

#else

#define PIN(a, b) P##a##b
enum {
  PIN_LIST
  NUM_DIGITAL_PINS,
};
#undef PIN

#endif

#include "lm3sgpio.h"
#endif
