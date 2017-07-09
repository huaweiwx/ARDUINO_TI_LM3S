/*
  LED.cpp - LED library
  huaweiwx@sina.com 2017.  All right reserved.

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
*/
#include <arduino.h>
#include "led.h"

extern "C"{	
  void LEDFlash(uint8_t pin,uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on){
	for(uint8_t i=cnt;i>0;i--){
		if(timeon){
		  digitalWrite(pin,on);
		  delay(timeon);
		}  
		if(timeoff){
		  digitalWrite(pin,(on?LOW:HIGH));
		  delay(timeoff);
		}   
	}
 }
}  //extern "C"{

LEDClass::LEDClass(uint8_t pin, uint8_t on){
	this->_pin = pin;
	this->_on =  on?HIGH:LOW;
	this->_off = on?LOW:HIGH;
	this->_status = this->_off;
	pinMode(pin,OUTPUT);
}

void LEDClass::Init(void){  
	pinMode(this->_pin,OUTPUT);	
}

void LEDClass::On(void){
	digitalWrite(this->_pin,this->_on);
	this->_status = this->_on;
}

void LEDClass::Off(void){
	digitalWrite(this->_pin,this->_off);
	this->_status = this->_off;
}

void LEDClass::Toggle(void){
	if(this->_status == this->_off) this->On();
	else this->Off();
	
}

void LEDClass::Flash(uint16_t timeon,uint16_t timeoff,uint8_t cnt){
	LEDFlash(this->_pin, timeon,timeoff,cnt,this->_on);
	this->_status = this->_off;
};