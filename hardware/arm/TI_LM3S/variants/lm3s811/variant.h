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
#ifndef _VARIANT_H_
#define _VARIANT_H_

#define __LM3S811__

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
 #define  BOARD_NR_LED  2 
 #define  LED_BUILTIN   PC5
 #define  LED1_BUILTIN   PD7
 #define  LED_BUILTIN_MASK  0x00
 
 #define  BOARD_NR_KEY	1
 #define  BUTTON  PC4
 #define  BUTTON_MASK   0x00
 #define  KEYS_LIST BUTTON 
 
#elif defined(ARDUINO_LM3S811EVB)
 #define  BOARD_NR_LED   7 
 #define  LED_BUILTIN    PC5
 #define  LED1_BUILTIN   PB0
 #define  LED2_BUILTIN   PB1
 #define  LED3_BUILTIN   PD1
 #define  LED4_BUILTIN   PE0
 #define  LED5_BUILTIN   PD0
 #define  LED6_BUILTIN   PE1
 #define  LED_BUILTIN_MASK  0x00
  
 #define  BOARD_NR_KEY	1
 #define  BUTTON  PC4
 #define  BUTTON_MASK   0x00

#else
 #warring "board not def"	
#endif


#endif 
