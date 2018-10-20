/*
  variant.h  for  LM3S8962   by huaweiwx@sina.com   2015.7.15					*

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

#define __LM3S8962__

#define NUM_ANALOG_INPUTS (4+1)     //TS A0~3

static const uint8_t A0 =  1;
static const uint8_t A1 =  2;
static const uint8_t A2 =  3;
static const uint8_t A3 =  4;

#define  BOARD_NR_LED  3 
#define  LED_BUILTIN   PF_0
#define  LED1_BUILTIN  PF_2
#define  LED2_BUILTIN  PF_3
#define  LED_BUILTIN_MASK  0x00

#endif 
