/* Tone.cpp

  A Tone Generator Library - Modified for Energia
  Implements up to 3 (software) PWM outputs using TIMERA0 compare registers and IRQ. 
  Can use any digital output pin for pulse generation

  (c) 2012 - Peter Brier.

  Based on code Originally written by Brett Hagman

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Version Modified By Date     Comments
------- ----------- -------- --------
0001    B Hagman    09/08/02 Initial coding
0002    B Hagman    09/08/18 Multiple pins
0003    B Hagman    09/08/18 Moved initialization from constructor to begin()
0004    B Hagman    09/09/26 Fixed problems with ATmega8
0005    B Hagman    09/11/23 Scanned prescalars for best fit on 8 bit timers
                    09/11/25 Changed pin toggle method to XOR
                    09/11/25 Fixed timer0 from being excluded
0006    D Mellis    09/12/29 Replaced objects with functions
0007    M Sproul    10/08/29 Changed #ifdefs from cpu to register
0008    P Brier     12/05/28 Modified for TI MSP430 processor
0009    P Brier     12/05/29 Fixed problem with re-init of expired tone
 *************************************************/

#include "wiring_private.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/interrupt.h"
#include "inc/pin_map.h"
#include "inc/timer.h"
#include "inc/sysctl.h"

// tone_duration:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new play() called)

static uint8_t tone_state = 0; // 0==not initialized, 1==timer running
static uint8_t current_pin = 0;
static long g_duration = 0;
static uint8_t tone_timer = 0;

void
ToneIntHandler(void)
{
    TimerIntClear(TIMER4_BASE, TIMER_A);

    //End of tone duration
    if(--g_duration <= 0) {
        	noTone(current_pin);
    		TimerIntDisable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
    		TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
    		TimerDisable(TIMER4_BASE, TIMER_A);
    }

}

/**
 *** tone() -- Output a tone (50% Dutycycle PWM signal) on a pin
 ***  pin: This pin is selected as output
 ***  frequency: [Hz]
 **   duration: [milliseconds], if duration <=0, then we output tone continuously,
 **   otherwise tone is stopped after this time (output = 0)
 **/

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
	if (_pin > NUM_DIGITAL_PINS) return;
//    uint8_t port = digitalPinToPort(_pin);
    if (tone_state == 0 || _pin == current_pin) {

    	//Setup PWM
    	current_pin = _pin;
        tone_timer = digitalPinToTimer(_pin);
        tone_state = 1;
        g_duration = duration;
        PWMWrite(_pin, 256, 128, frequency);

        //Setup interrupts for duration, interrupting at 1kHz
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        IntMasterEnable();
        TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER4_BASE, TIMER_A, F_CPU/1000);
        IntEnable(INT_TIMER4A);
        TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
        TimerEnable(TIMER4_BASE, TIMER_A);
    }
}

void tone(uint8_t _pin, unsigned int frequency)
{

	if (_pin > NUM_DIGITAL_PINS) return;
	
//    uint8_t port = digitalPinToPort(_pin);
    if(tone_state == 0 || _pin == current_pin) {
        tone_timer = digitalPinToTimer(_pin);
        PWMWrite(_pin, 256, 128, frequency);
        tone_state = 1;
    }

}

/*
 * noTone() - Stop outputting the tone on a pin
 */
void noTone(uint8_t _pin)
{

	uint8_t timer = digitalPinToTimer(_pin);

    if(timer == tone_timer) {
		uint32_t timerBase = getTimerBase(timerToOffset(timer));
		uint32_t timerAB = TIMER_A << timerToAB(timer);
		TimerIntDisable(timerBase, TIMER_TIMA_TIMEOUT << timerToAB(tone_timer));
		TimerIntClear(timerBase, TIMER_TIMA_TIMEOUT << timerToAB(tone_timer));
		TimerDisable(timerBase, timerAB);
		tone_state = 0;
		g_duration = 0;
		pinMode(_pin, OUTPUT);
		digitalWrite(_pin, LOW);
    }

}


