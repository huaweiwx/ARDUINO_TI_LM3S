/*
 ************************************************************************
 *	wiring_digital.c
 *
 *	Arduino core files for lm3s811 modify by huawei huaweiwx@sina.com 2015
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************
  Derived from:
  wiring_digital.c - digital input and output functions
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

#define ARDUINO_MAIN
#include "part.h"

#include "wiring_private.h"
#include "inc/gpio.h"
#include "inc/sysctl.h"
//#define GPIO_LOCK_KEY_DD        0x4C4F434B    //lm3s811 not this regs

void pinMode(uint8_t ucPin, uint8_t mode)
{
	if (ucPin > NUM_DIGITAL_PINS) return;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t port = digitalPinToPort(ucPin);
//  if (port == NOT_A_PIN) return;
	
    uint32_t portBase = (uint32_t) portBASERegister(port);
//    volatile uint32_t *lock = portLOCKRegister(port);
//    volatile uint32_t *cr = portCRRegister(port);    
     
    if (mode == INPUT) {  //mode 0
        GPIOPinTypeGPIOInput(portBase, bit);
    } else if (mode == INPUT_PULLUP) {  //mode 2
//        *lock = GPIO_LOCK_KEY_DD;
//        *cr |= bit;
//        *lock = 0;
        GPIODirModeSet(portBase, bit, GPIO_DIR_MODE_IN);
        GPIOPadConfigSet(portBase, bit,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//        *cr &= ~bit;
    } else if (mode == INPUT_PULLDOWN) { //mode 3
//        *lock = GPIO_LOCK_KEY_DD;
//        *cr |= bit;
//        *lock = 0;
        GPIODirModeSet(portBase, bit, GPIO_DIR_MODE_IN);
        GPIOPadConfigSet(portBase, bit,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//        *cr &= ~bit;
    } else {  //mode == OUTPUT 1
        GPIOPinTypeGPIOOutput(portBase, bit);
    }

}

int digitalRead(uint8_t ucPin)
{
	if (ucPin > NUM_DIGITAL_PINS) return LOW;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t port = digitalPinToPort(ucPin);
//    if (port == NOT_A_PIN) return LOW;
	
    uint32_t portBase = (uint32_t) portBASERegister(port);
    if(GPIOPinRead(portBase, bit)){
    	return HIGH;
    }
    return LOW;
}

void digitalWriteHigh(uint8_t ucPin)
{
	if (ucPin > NUM_DIGITAL_PINS) return;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t port = digitalPinToPort(ucPin);
	uint32_t portBase = (uint32_t) portBASERegister(port);
    GPIOPinWrite(portBase, bit, bit);
}


void digitalWriteLow(uint8_t ucPin)
{
	if (ucPin > NUM_DIGITAL_PINS) return;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t port = digitalPinToPort(ucPin);
	uint32_t portBase = (uint32_t) portBASERegister(port);
    GPIOPinWrite(portBase, bit, 0);
}


void digitalWrite(uint8_t ucPin, uint8_t val)
{
	if (ucPin > NUM_DIGITAL_PINS) return;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t mask = val ? bit : 0;
    uint8_t port = digitalPinToPort(ucPin);
//    if (port == NOT_A_PORT) return;
    
	uint32_t portBase = (uint32_t) portBASERegister(port);
    GPIOPinWrite(portBase, bit, mask);
}

//add by huawei
void digitalToggle(uint8_t ucPin)
{
	if (ucPin > NUM_DIGITAL_PINS) return;
    uint8_t bit = digitalPinToBitMask(ucPin);
    uint8_t port = digitalPinToPort(ucPin);
//    if (port == NOT_A_PORT) return;
	
    uint32_t portBase = (uint32_t) portBASERegister(port);
	uint8_t val = (GPIOPinRead(portBase, bit));
    uint8_t mask = val ? 0 : bit;	
    GPIOPinWrite(portBase, bit, mask);
}

extern void digitalFlash(uint8_t ucPin, uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on){
	volatile uint8_t i;
           
	for( i=cnt;i>0;i--){
		if(timeon >0 ){
          digitalWrite(ucPin, on);
		  delay(timeon);
		}  
		if(timeoff >0){
		  digitalWrite(ucPin, (on?LOW:HIGH));
		  delay(timeoff);
		}   
	}
}

#ifdef LED_BUILTIN
extern void ledFlash(uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on){
	digitalFlash(LED_BUILTIN, timeon,timeoff, cnt, on);
}

/* USER CODE END 2 */
extern void errorFlash(uint8_t id){
	uint8_t i1 = id >>8;
	uint8_t i2 = id &0xf;
	while(1){
      ledFlash(10, 490,i1,1);
	  ledFlash(500,500,i2,1);
	}	
}
#endif
