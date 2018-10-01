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
#ifndef _VARIANT_H_
#define _VARIANT_H_

#define __LM3S8962__

#define NUM_ANALOG_INPUTS (4+1)     //TS A0~3
//
// Pin names based on the silkscreen
//

static const uint8_t A0 =  1;
static const uint8_t A1 =  2;
static const uint8_t A2 =  3;
static const uint8_t A3 =  4;

//arduino 1.6.6 define add by hw
#define  BOARD_NR_LED  3 
#define  LED_BUILTIN   PF0
#define  LED1_BUILTIN  PF2
#define  LED2_BUILTIN  PF3
#define  LED_BUILTIN_MASK  0x00
#define  LEDS_LIST LED1_BUILTIN,LED2_BUILTIN

#endif 
