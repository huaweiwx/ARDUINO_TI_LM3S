/*
  LED.h - LED library head file
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

#ifndef __LEDLIB_h
#define __LEDLIB_h

#ifdef __cplusplus
extern "C"{
#endif

void LEDFlash(uint8_t pin,uint16_t timeon,uint16_t timeoff,uint8_t cnt,uint8_t on);  //add by huawei 2015.3

#ifdef __cplusplus
} //extern "C"{
#endif
class LEDClass
{
 public:
	LEDClass(uint8_t pin=LED_BUILTIN, uint8_t on=HIGH);
	void Init(void);
	void On(void);
	void Off(void);
	void Toggle(void);
	void Flash(uint16_t timeon,uint16_t timeoff,uint8_t cnt);
	int  pinNum(void) { return this->_pin; }
	int  onVal(void)  { return this->_on; }
	int  isOn(void)   { return this->_status; }
  private:
    volatile uint8_t _off, _on, _pin,_status;
};

#endif

