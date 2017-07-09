/*
 ************************************************************************
 *	wiring.c
 *
 *	Arduino core files for MSP430
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

 // For LM3S811 add by huaweiwx@sina.com 2015.7.17

 
#include "arduino.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"

#include "inc/sysctl.h"
#include "inc/timer.h"
#include "inc/systick.h"
#include "inc/interrupt.h"

static void (*SysTickCbFuncs[8])(uint32_t ui32TimeMS);

#define SYSTICKHZ               1000UL
#define SYSTICKMS               (1000UL / SYSTICKHZ)

static inline void SysTickMode_Run(void);

static volatile unsigned long milliseconds = 0;
#define SYSTICK_INT_PRIORITY    0x80
void timerInit()
{

      SysCtlLDOSet(SYSCTL_LDO_2_75V);    //配置PLL前需将LDO电压设置为2.75V  
#if defined(PART_LM3S8962)
      SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |SYSCTL_XTAL_8MHZ);
#elif defined(PART_LM3S811)	  
      SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |SYSCTL_XTAL_6MHZ);
#else
#  warning "mcu undefined!"
#endif
    //
    //  SysTick is used for delay() and delayMicroseconds()
    //
      SysTickPeriodSet(SysCtlClockGet()/ SYSTICKHZ);
      SysTickEnable();


    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);
	
    SysTickIntEnable();
    IntMasterEnable();
 
//    SysCtlPIOSCCalibrate(SYSCTL_PIOSC_CAL_FACT);  // Factory-supplied calibration used

}

unsigned long micros(void)
{
	return (milliseconds * 1000) + ( ((SysCtlClockGet()/ SYSTICKHZ) - SysTickValueGet()) / (SysCtlClockGet()/1000000));
}

unsigned long millis(void)
{
	return milliseconds;
}

void delayMicroseconds(unsigned int us)
{
	// Systick timer rolls over every 1000000/SYSTICKHZ microseconds 
	if (us > (1000000UL / SYSTICKHZ - 1)) {
		delay(us / 1000);  // delay milliseconds
		us = us % 1000;     // handle remainder of delay
	};

	// 24 bit timer - mask off undefined bits
	unsigned long startTime = HWREG(NVIC_ST_CURRENT) & NVIC_ST_CURRENT_M;

	unsigned long ticks = (unsigned long)us * (SysCtlClockGet()/1000000UL);
	volatile unsigned long elapsedTime;

	if (ticks > startTime) {
		ticks = (ticks + (NVIC_ST_CURRENT_M - (unsigned long)SysCtlClockGet()/ SYSTICKHZ)) & NVIC_ST_CURRENT_M;
	}

	do {
		elapsedTime = (startTime-(HWREG(NVIC_ST_CURRENT) & NVIC_ST_CURRENT_M )) & NVIC_ST_CURRENT_M;
	} while(elapsedTime <= ticks);
}

void delay(uint32_t millis)
{
	unsigned long i;
	for(i=0; i<millis*2; i++){
		delayMicroseconds(500);
	}
}


void registerSysTickCb(void (*userFunc)(uint32_t))
{
	uint8_t i;
	for (i=0; i<8; i++) {
		if(!SysTickCbFuncs[i]) {
			SysTickCbFuncs[i] = userFunc;
			break;
		}
	}
}

void SysTickIntHandler(void)
{
	milliseconds++;

	uint8_t i;
	for (i=0; i<8; i++) {
		if (SysTickCbFuncs[i])
			SysTickCbFuncs[i](SYSTICKMS);
	}
}


__attribute__((always_inline))
static inline void SysTickMode_Run(void)
{
	HWREG(NVIC_ST_RELOAD) = SysCtlClockGet()/ SYSTICKHZ - 1;
	HWREG(NVIC_ST_CURRENT) = 0;
}

