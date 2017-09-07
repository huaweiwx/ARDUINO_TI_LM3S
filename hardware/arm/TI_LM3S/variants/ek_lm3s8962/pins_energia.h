/***********************************************************************
 *	pins_energia.h
 *
 *	Energia core files for LM3S8962
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *     Contribution: Rei VILO
 *
 ***********************************************************************
  Derived from:
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 ************************************************************************
 *    for  LM3S8962  by huaweiwx@sina.com   2016.10.15					*
 ***********************************************************************/
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define __LM3S8962__

#ifndef BV 
#define BV(x) (1 << (x)) 
#endif

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/adc.h"



//pin_map define for lm3s8962
enum {
	PA0,PA1,PA2,PA3,PA4,PA5,PA6, PA7,   //0~ 7
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,_PB7,   //8~ 15
   _PC0,_PC1,_PC2,_PC3,PC4,PC5,PC6,PC7,   //16~23
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,   //24~31
	PE0,PE1,PE2,PE3,				  //32~35	
	PF0,PF1,PF2,PF3,				  //36~39
	PG0,PG1,						  //40~41
    NUM_DIGITAL_PINS,
};
#define  BOARD_NR_GPIO_PINS    NUM_DIGITAL_PINS    /*兼容arduino*/

#define NUM_ANALOG_INPUTS (4+1)     //TS A0~3

//
// Pin names based on the silkscreen
//

static const uint8_t A0 =  1;
static const uint8_t A1 =  2;
static const uint8_t A2 =  3;
static const uint8_t A3 =  4;


#define  LED1   PF0
#define  LED2   PF2
#define  LED3   PF3

//arduino 1.6.6 define add by hw
#ifndef LED_BUILTIN
 #define  LED_BUILTIN    LED1 // 
#endif

#ifdef ARDUINO_MAIN

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
	T2B,			//91  PB5/C0o
	NOT_ON_TIMER,   //90  PB6/C0+
	NOT_ON_TIMER,   //89  PB7/TRST
	
	NOT_ON_TIMER,   //80  PC0/TCK/SWCLK
	NOT_ON_TIMER,   //79  PC1/TMS/SWDIO
	NOT_ON_TIMER,   //78  PC2/TDI
	NOT_ON_TIMER,   //77  PC3/TDO/SWO
	NOT_ON_TIMER,   //25 14 PC4/PhA0
	T0B,			//24 13 PC5 
	T1B,			//23 12 PC6/PhB0
	NOT_ON_TIMER,	//22 11 PC7/
	
	NOT_ON_TIMER,   //10 0 PD0/CAN0Rx
	NOT_ON_TIMER,   //11 1 PD1/CAN0Tx
	NOT_ON_TIMER,   //12 2 PD2/U1RX
	NOT_ON_TIMER,   //13 3 PD3/U1TX
	T0A,			//95 PD4/CCP0
	T1A,			//96  PD5
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
	PC,	  //24 13 PC5 
	PC,	  //23 12 PC6/PhB0
	PC,	  //22 11 PC7/

	PD,   //10 5 PD0/CAN0Rx
	PD,   //11 6 PD1/CAN0Tx
	PD,   //12 7 PD2/U1RX
	PD,   //13 8 PD3/U1TX
	PD,	  //95 43 PD4/CCP0
	PD,	  //96 44 PD5
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
	BV(5),	  //24 13 PC5 
	BV(6),	  //23 12 PC6/PhB0
	BV(7),	  //22 11 PC7/
	
	BV(0),   //10 5 PD0/CAN0Rx
	BV(1),   //11 6 PD1/CAN0Tx
	BV(2),   //12 7 PD2/U1RX
	BV(3),   //13 8 PD3/U1TX
	BV(4),	  //95 43 PD4/CCP0
	BV(5),	  //96 44 PD5
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
/*		CCP0_PORT,
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
        ADC_CTL_CH1,	//  2 - ADC1
        ADC_CTL_CH2,    //  3 - ADC2
        ADC_CTL_CH3, 	//  4 - ADC3
};
#endif

#endif 
