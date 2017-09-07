/***********************************************************************
 *	pins_energia.h
 *
 *	Energia core files for LM3S811 
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
 *    for  LM3S811   by huaweiwx@sina.com   2015.7.15					*
 ***********************************************************************/
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define __LM3S811__


#ifndef BV 
#define BV(x) (1 << (x)) 
#endif

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/adc.h"



//pin_map define
enum {
	PA0,PA1,PA2,PA3,PA4,PA5,   			//0~ 5
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,_PB7,    //6~ 13
//	_PC0,_PC1,_PC2,_PC3,PC4,PC5,PC6,PC7,    //14~21
	                PC4,PC5,PC6,PC7,    //14~21
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,    //22~29
	PE0,PE1,				            //30~31	
	BOARD_NR_GPIO_PINS,
};

#define   NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS      /*兼容arduino*/

// #define  PC3  20  //PC3/TDO/SWO    //禁用 防lock
// #define  PC2  21  //PC2/TDI
// #define  PC1  22  //PC1/TMS/SWDIO
// #define  PC0  23  //PC0/TCK/SWCLK
// #define  PB7  24  //PB7/TRST

#define NUM_ANALOG_INPUTS (4+1)    //TS A0~3

//
// Pin names based on the silkscreen
//
static const uint8_t A0 =  1;
static const uint8_t A1 =  2;
static const uint8_t A2 =  3;
static const uint8_t A3 =  4;
//RST  5;
//LDO  6;
//VDD  7;
//GND  8;
//OSC0 9;
//OSC1 10;

#if defined(ARDUINO_LM3S811ND)
 #define  LED1   PC5
 #define  LED2   PD7
 #define  PUSH1  PC4

#elif defined(ARDUINO_LM3S811EVB)
 #define  LED1   PC5
 #define  LED2   PB0
 #define  LED3   PB1
 #define  LED4   PD1
 #define  LED5   PE0
 #define  LED6   PD0
 #define  LED7   PE1
 
 #define  PUSH1  PC4
#else
 #warring "board not def"	
#endif

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
	T2B,			//43  PB5
	NOT_ON_TIMER,   //42  PB6/C0+
	NOT_ON_TIMER,   //41  PB7/TRST
	
//	NOT_ON_TIMER,   //40  PC0/TCK/SWCLK
//	NOT_ON_TIMER,   //39  PC1/TMS/SWDIO
//	NOT_ON_TIMER,   //38  PC2/TDI
//	NOT_ON_TIMER,   //37  PC3/TDO/SWO
	NOT_ON_TIMER,   //14  PC4
	T0B,			//13  PC5 
	T1B,			//12  PC6
	T2A,			//11  PC7/CCP4
	
	NOT_ON_TIMER,   //25  PD0/PWM0
	NOT_ON_TIMER,   //26  PD1/PWM1
	NOT_ON_TIMER,   //27  PD2/U1RX
	NOT_ON_TIMER,   //28  PD3/U1TX
	T0A,			//45  PD4
	T1A,			//46  PD5
	NOT_ON_TIMER,   //47  PD6/FAULT
	NOT_ON_TIMER,   //48  PD7/C0o
	
	NOT_ON_TIMER,   //35  PE0/PWM4
	NOT_ON_TIMER,   //36  PE1/PWM5
};

const uint8_t digital_pin_to_port[] = {
	PA, 			//17 PA0/U0RX
	PA,  			//18 PA1/U0TX
	PA,  			//19 PA2/SSICLK
	PA,   			//20 PA3/SSIFss
	PA,  			//21 PA4/SSIRX
	PA,  			//22 PA5/SSITX
	
	PB, 			//29 PB0/PWM2
	PB, 			//30 PB1/PWM1
	PB, 		    //33 PB2/I2CSCL
	PB,			    //34 PB3/I2CSDA
	PB,			    //44  PB4/C0-
	PB,			    //43  PB5/CCP5
	PB,			    //42  PB6/C0+
	PB,			    //41  PB7/TRST
	
//	PC,			//40  PC0/TCK/SWCLK
//	PC,			//39  PC1/TMS/SWDIO
//	PC,			//38  PC2/TDI
//	PC,			//37  PC3/TDO/SWO
	PC,			//14	PC4
	PC,			//13;   //PC5/CCP1 
	PC,			//12	PC6/CCP3
	PC,			//11	PC7/CCP4
	
	PD,			//25  PD0/PWM0
	PD,			//26  PD1/PWM1
	PD,			//27  PD2/U1RX
	PD,			//28  PD3/U1TX
	PD,			//45  PD4/CCP0
	PD,			//46  PD5/CCP2
	PD,			//47  PD6/FAULT
	PD,			//48  PD7/C0o
	
	PE,			//35  PE0/PWM4
	PE,			//36  PE1/PWM5
};
const uint8_t digital_pin_to_bit_mask[] = {
	BV(0),			//17  PA0/U0RX
	BV(1),			//18	PA1/U0TX
	BV(2),			//19	PA2/SSICLK
	BV(3),			//20	PA3//SSIFss
	BV(4),			//21   PA4//SSIRX
	BV(5),			//22	PA5//SSITX
	
	BV(0),			//29  PB0/PWM2
	BV(1),			//30  PB1/PWM1
	BV(2),			//33	PB2/I2CSCL
	BV(3),			//34	PB3/I2CSDA
	BV(4),			//44	PB4/C0-
	BV(5),			//43	PB5/CCP5
	BV(6),			//42	PB6/C0+
	BV(7),		//41	PB7/TRST
	
//	BV(0),		//40	PC0/TCK/SWCLK
//	BV(1),		//39	PC1/TMS/SWDIO
//	BV(2),		//38	PC2/TDI
//	BV(3),		//37	PC3/TDO/SWO
	BV(4),			//14  PC4 
	BV(5),			//13  PC5/CCP1 
	BV(6),			//12  PC6/CCP3
	BV(7),			//11  PC7/CCP4
	
	BV(0),			//25  PD0/PWM0
	BV(1),			//26  PD1/PWM1
	BV(2),			//27  PD2/U1RX
	BV(3),			//28  PD3/U1TX
	BV(4),			//45	PD4/CCP0
	BV(5),			//46	PD5/CCP2
	BV(6),			//47	PD6/FAULT
	BV(7),			//48	PD7/C0o
	
	BV(0),			//35	PE0/PWM4
	BV(1),			//36	PE1/PWM5
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
        ADC_CTL_CH1,	//  2 - ADC1
        ADC_CTL_CH2,    //  3 - ADC2
        ADC_CTL_CH3, 	//  4 - ADC3
};
#endif

#endif 
