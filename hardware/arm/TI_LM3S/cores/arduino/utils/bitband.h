/* 
  huaweiwx@sina.com 2017.  All right reserved.

  for reg io opration
  for all avr chips 2017.5.28
 
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

#ifndef __BITBAND_H__
#define __BITBAND_H__
#include "Constpin.h"
#include "wiring_private.h"
#ifdef __cplusplus

///////////////////////////////
// The following functions are meant to be used with compile time constant parameters
#define PIN(a, b)  &PORT##a
static const uint16_t variant_gpiopin_port_static[] = {
   PIN_LIST
};  
#undef PIN
 
#define PIN(a, b)   &PIN##a
static const uint16_t variant_gpiopin_pin_static[] = {
  PIN_LIST
};  
#undef PIN

#define PIN(a, b)  &DDR##a
static const uint16_t variant_gpiopin_ddr_static[] = {
  PIN_LIST
};

#define PIN(a, b)  b
static const uint8_t variant_gpiopin_bit_static[] = {
  PIN_LIST
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"

template<const uint8_t PinNumber>
class FAST_PIN{
 public:
  //----------------------------------------------------------------------------
  constexpr const uint16_t port(void){return variant_gpiopin_port_static[PinNumber];};
  constexpr const uint16_t pin(void){return variant_gpiopin_pin_static[PinNumber];};
  constexpr const uint16_t ddr(void){return variant_gpiopin_ddr_static[PinNumber];};
  constexpr const uint8_t  ibit(void){return variant_gpiopin_bit_static[PinNumber];};
  
  inline FAST_PIN & operator = (bool value) __attribute__((always_inline)) {
    this->write(value);
    return *this;
  }
  
  inline FAST_PIN & operator << (bool value) __attribute__((always_inline)) {
    this->write(value);
    return *this;
  }

  template<class T> 
  inline FAST_PIN & operator >> (T &value){
    value = this->read();
    return *this;
  }

  inline __attribute__((always_inline))
  void write(bool val){
  	uint8_t oldSREG = SREG;
	cli();
	if (val == LOW) {
		*(volatile uint8_t *)(this->port())  &= ~ this->ibit();
	} else {
		*(volatile uint8_t *)(this->port()) |= this->ibit();
	}
	SREG = oldSREG;
  }

  inline __attribute__((always_inline))
  void high(){this->write(HIGH);}

  inline __attribute__((always_inline))
  void low(){this->write(HIGH);}

  inline operator bool () const __attribute__((always_inline)) {
	return this->read();
  }

  inline __attribute__((always_inline))
  bool read() const{
    return (*(volatile uint8_t *)(this->pin()) & this->ibit)?HIGH:LOW;
  }
  
  inline void operator  !() __attribute__((always_inline)) {this->toggle();}

/*----- comptabled with DigitalPin ----------*/
  inline __attribute__((always_inline))
  void toggle(){
  	uint8_t oldSREG = SREG;
	cli();
	*(volatile uint8_t *)(this->port()) ^= this->ibit();
	SREG = oldSREG;
  }
  
  inline __attribute__((always_inline))
  void config(uint8_t mode, bool level) {
      this->write(level);
	  this->mode(mode);
  }

  inline __attribute__((always_inline))
  void mode(uint8_t mode){
	uint8_t oldSREG = SREG;
    cli();
	if (mode == INPUT) {
		*(volatile uint8_t *)(this->ddr()) &= ~this->ibit();
		*(volatile uint8_t *)(this->port()) &= ~this->ibit();
	} else if (mode == INPUT_PULLUP) {
		*(volatile uint8_t *)(this->ddr()) &= ~this->ibit();
		*(volatile uint8_t *)(this->port()) |= this->ibit();
	} else {
		*(volatile uint8_t *)(this->ddr()) |= this->ibit();
	}
	SREG = oldSREG;
	}

  inline __attribute__((always_inline))
  void attach(voidFuncPtr callback, uint8_t mode){
	   attachInterrupt(PinNumber, callback, mode);
  }
  
  inline __attribute__((always_inline))
  void detach(void){
	  detachInterrupt(PinNumber);
  }
};

#pragma GCC diagnostic pop
#define BB_PIN FAST_PIN

#endif //__cplusplus
#endif //__BITBAND_H__
