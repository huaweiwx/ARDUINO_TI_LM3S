/*
  variant.h  for  LM3S811   by huaweiwx@sina.com   2015.7.15					*

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
 */
#ifndef _VARIANT_H_
#define _VARIANT_H_

#define __LM3S811__

#define NUM_ANALOG_INPUTS (4+1)    //TS A0~3

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
 #define  LED_BUILTIN    PC_5
 #define  LED1_BUILTIN   PD_7
 #define  LED_BUILTIN_MASK  0x00
 
 #define  BOARD_NR_KEY	1
 #define  BUTTON         PC_4
 #define  BUTTON_MASK    0x00
 #define  KEYS_LIST      BUTTON 
 
#elif defined(ARDUINO_LM3S811EVB)
 #define  BOARD_NR_LED   7 
 #define  LED_BUILTIN    PC_5
 #define  LED1_BUILTIN   PB_0
 #define  LED2_BUILTIN   PB_1
 #define  LED3_BUILTIN   PD_1
 #define  LED4_BUILTIN   PE_0
 #define  LED5_BUILTIN   PD_0
 #define  LED6_BUILTIN   PE_1
 #define  LED_BUILTIN_MASK  0x00
  
 #define  BOARD_NR_KEY	1
 #define  BUTTON  		PC_4
 #define  BUTTON_MASK   0x00

#else
 #warring "board not def"	
#endif

#endif 
