/*
 ************************************************************************
 *	wiring_analog.c
 *
 *	Arduino core files for LM3S811 by huaweiwx@sina.com
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring_analog.c - analog input and output
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
 
//2015 FOR LM3SXX   AND change ROM_ to   by huaweiwx@sina.com

#include "wiring_private.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/adc.h"
#include "inc/gpio.h"
#include "inc/sysctl.h"
#include "inc/pin_map.h"

#include "inc/timer.h"


#define PWM_MODE 0x20A


uint32_t getTimerBase(uint32_t offset) {
    return (TIMER0_BASE + (offset << 12));
}

uint8_t getTimerInterrupt(uint8_t timer) {
    return(INT_TIMER0A + timer);
}

void enableTimerPeriph(uint32_t offset) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0 + offset);
}

//
//empty function due to single reference
//
#pragma GCC diagnostic ignored "-Wunused-parameter"
void analogReference(uint16_t mode) {
}

void PWMWrite(uint8_t ucPin, uint32_t analog_res, uint32_t duty, unsigned int freq) {
    if (duty == 0) {
    	pinMode(ucPin, OUTPUT);
        digitalWrite(ucPin, LOW);
    }
    else if (duty >= analog_res) {
    	pinMode(ucPin, OUTPUT);
    	digitalWrite(ucPin, HIGH);
    }
    else {
        uint8_t bit = digitalPinToBitMask(ucPin); // get ucPin bit
        uint8_t port = digitalPinToPort(ucPin);   // get ucPin port
        uint8_t timer = digitalPinToTimer(ucPin);
        uint32_t portBase = (uint32_t) portBASERegister(port);
        uint32_t offset = timerToOffset(timer);
        uint32_t timerBase = getTimerBase(offset);
        uint32_t timerAB = TIMER_A << timerToAB(timer);

        uint32_t periodPWM = SysCtlClockGet()/freq;

        enableTimerPeriph(offset);
        GPIOPinConfigure(timerToPinConfig(timer));
        GPIOPinTypeTimer((long unsigned int) portBase, bit);

        //
        // Configure for half-width mode, allowing timers A and B to
        // operate independently
        //
        HWREG(timerBase + TIMER_O_CFG) = 0x04;

        if(timerAB == TIMER_A) {
        	HWREG(timerBase + TIMER_O_CTL) &= ~TIMER_CTL_TAEN;
        	HWREG(timerBase + TIMER_O_TAMR) = PWM_MODE;
        }
        else {
        	HWREG(timerBase + TIMER_O_CTL) &= ~TIMER_CTL_TBEN;
        	HWREG(timerBase + TIMER_O_TBMR) = PWM_MODE;
        }
        TimerLoadSet(timerBase, timerAB, periodPWM);
        TimerMatchSet(timerBase, timerAB,
                (analog_res-duty)*periodPWM/analog_res);

        //
        // If using a 16-bit timer, with a periodPWM > 0xFFFF,
        // need to use a prescaler
        //

        TimerEnable(timerBase, timerAB);
    }
}
void analogWrite(uint8_t ucPin, int val) {
    //
    //  duty cycle(%) = val / 255;
    //  Frequency of 490Hz specified by Arduino API
    //
    PWMWrite(ucPin, 255, val, 490);
}

uint16_t analogRead(uint8_t ucPin) {
//    uint8_t  port = digitalPinToPort(ucPin);
    uint16_t value[1];
    uint32_t channel = digitalPinToADCIn(ucPin);
    if (channel == NOT_ON_ADC) { //invalid ADC ucPin
        return 0;
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
#if 0
    if(channel != ADC_CTL_TS)
        GPIOPinTypeADC((uint32_t) portBASERegister(port), digitalPinToBitMask(ucPin));
#endif
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, channel | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    ADCIntClear(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC0_BASE, 3);
    while(!ADCIntStatus(ADC0_BASE, 3, false)) {
    }
	ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, (unsigned long*) value);
    return value[0];
}

uint16_t getTemp(void){
    float tmp;
	tmp = analogRead(0);
	tmp = tmp / 1024*3.3;
	tmp = (2.7-tmp)*75 - 55;
	return tmp*100;
}	