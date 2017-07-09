/*
 ************************************************************************
 *	WInterrupts.c
 *
 *	Arduino core files for MSP430
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:

  WInterrupts.c Part of the Wiring project - http://wiring.uniandes.edu.co

  Copyright (c) 2004-05 Hernando Barragan

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

  Modified 24 November 2006 by David A. Mellis
  Modified 1 August 2010 by Mark Sproul
  Modified 11 August 2015 by huaweiwx@sina.com

 */
#include <arduino.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_ints.h"
#include "inc/gpio.h"
#include "inc/interrupt.h"

#include "wiring_private.h"


static void (*cbFuncsA[8])(void);
static void (*cbFuncsB[8])(void);
static void (*cbFuncsC[8])(void);
static void (*cbFuncsD[8])(void);
static void (*cbFuncsE[8])(void);


void GPIOXIntHandler(uint32_t base, void (**funcs)(void))
{
	uint32_t i;
	uint32_t isr = GPIOPinIntStatus(base, true);

	GPIOPinIntClear(base, isr);

	for (i=0; i<8; i++, isr>>=1) {
		if ((isr & 0x1) == 0)
			continue;
		if (funcs[i])
			funcs[i]();
	}
}

void GPIOAIntHandler(void)
{
	GPIOXIntHandler(GPIO_PORTA_BASE, cbFuncsA);
}

void GPIOBIntHandler(void)
{
	GPIOXIntHandler(GPIO_PORTB_BASE, cbFuncsB);
}

void GPIOCIntHandler(void)
{
	GPIOXIntHandler(GPIO_PORTC_BASE, cbFuncsC);
}

void GPIODIntHandler(void)
{
	GPIOXIntHandler(GPIO_PORTD_BASE, cbFuncsD);
}

void GPIOEIntHandler(void)
{
	GPIOXIntHandler(GPIO_PORTE_BASE, cbFuncsE);
}

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
	uint32_t lm3sMode, i;

	uint8_t bit = digitalPinToBitMask(interruptNum);
	uint8_t port = digitalPinToPort(interruptNum);
	uint32_t portBase = (uint32_t) portBASERegister(port);

	switch(mode) {
	case LOW:
		lm3sMode = GPIO_LOW_LEVEL;
		break;
	case HIGH:
		lm3sMode = GPIO_HIGH_LEVEL;
		break;
	case CHANGE:
		lm3sMode = GPIO_BOTH_EDGES;
		break;
	case RISING:
		lm3sMode = GPIO_RISING_EDGE;
		break;
	case FALLING:
		lm3sMode = GPIO_FALLING_EDGE;
		break;
	default:
		return;
	}

	IntMasterDisable();
	GPIOPinIntClear(portBase, bit);
	GPIOIntTypeSet(portBase, bit, lm3sMode);
	GPIOPinIntEnable(portBase, bit);

	for (i=0; i<8; i++, bit>>=1) {
		if ((bit & 0x1) == 1)
			break;
	}

	switch(portBase) {
	case GPIO_PORTA_BASE:
		cbFuncsA[i] = userFunc;
		IntEnable(INT_GPIOA);
		break;
	case GPIO_PORTB_BASE:
		cbFuncsB[i] = userFunc;
		IntEnable(INT_GPIOB);
		break;
	case GPIO_PORTC_BASE:
		cbFuncsC[i] = userFunc;
		IntEnable(INT_GPIOC);
		break;
	case GPIO_PORTD_BASE:
		cbFuncsD[i] = userFunc;
		IntEnable(INT_GPIOD);
		break;
	}
	IntMasterEnable();
}

void detachInterrupt(uint8_t interruptNum)
{
	uint32_t i;
	uint8_t bit =  digitalPinToBitMask(interruptNum);
	uint8_t port = digitalPinToPort(interruptNum);
	uint32_t portBase = (uint32_t) portBASERegister(port);
	
    GPIOPinIntDisable(portBase, bit);
	for (i=0; i<8; i++, bit>>=1) {
		if ((bit & 0x1) == 1)
			break;
	}

	switch(portBase) {
	case GPIO_PORTA_BASE:
		cbFuncsA[i] = 0;
	case GPIO_PORTB_BASE:
		cbFuncsB[i] = 0;
	case GPIO_PORTC_BASE:
		cbFuncsC[i] = 0;
	case GPIO_PORTD_BASE:
		cbFuncsD[i] = 0;
	case GPIO_PORTE_BASE:
		cbFuncsE[i] = 0;
	}
}
