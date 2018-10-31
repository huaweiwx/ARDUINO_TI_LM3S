/*
  Copyright (c) 2018 huaweiwx@sina.com 2018.7.1

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LM3SGPIO_H__
#define __LM3SGPIO_H__

#include "utils/base.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
inline void pinMode(__ConstPin CPin, const uint32_t mode) {
  if (mode == INPUT) {
    GPIOPinTypeGPIOInput(CPin.port, CPin.pinMask);
  } else if (mode == INPUT_PULLUP) {  //mode 2
    GPIODirModeSet(CPin.port, CPin.pinMask, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(CPin.port, CPin.pinMask, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  } else if (mode == INPUT_PULLDOWN) { //mode 3
    GPIODirModeSet(CPin.port, CPin.pinMask, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(CPin.port, CPin.pinMask, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
  } else {  //mode == OUTPUT 1
    GPIOPinTypeGPIOOutput(CPin.port, CPin.pinMask);
  }
}

inline void digitalWriteHigh(__ConstPin CPin)
{
  GPIOPinWrite(CPin.port, CPin.pinMask, CPin.pinMask);
}

inline void digitalWriteLow(__ConstPin CPin)
{
  GPIOPinWrite(CPin.port, CPin.pinMask, 0);
}

template<typename T>
inline void digitalWrite(__ConstPin CPin, T val )
{
  GPIOPinWrite(CPin.port, CPin.pinMask, (val) ? CPin.pinMask:0);
}

template<typename T>
inline T digitalRead(__ConstPin CPin)
{
  if (GPIOPinRead(CPin.port, CPin.pinMask)) {
    return HIGH;
  }
  return LOW;
}

inline void digitalToggle(__ConstPin CPin)
{
  /* can add a section here to see if pin is readable */
  if (digitalRead(CPin))
    digitalWriteLow(CPin);
  else
    digitalWriteHigh(CPin);
}

/*gpio low layer interface class*/
class LL_PIN {
  public:
    constexpr LL_PIN(__ConstPin CPin): CPin(CPin) {};
    __ConstPin CPin;

    template<typename T = bool>
    inline  T read() {
      return digitalRead(CPin);
    }

    template<typename T>
    inline void write(T value) {
      digitalWrite(CPin, value);
    }

    template<typename T>
    inline LL_PIN & operator = (T value) {
      this->write(value);
      return *this;
    }
    template<typename T>
    inline LL_PIN & operator ^= (T value) {
      if(value) this->toggle();
      return *this;
    }

    LL_PIN& operator = (LL_PIN& rhs) {
      this->write(rhs.read());
      return *this;
    }

    inline __attribute__((always_inline))
    void high() {
      write(HIGH);
    }

    inline __attribute__((always_inline))
    void low() {
      write(LOW);
    }

    template<typename T>
    inline operator T () {
      return read();
    }

    inline void operator  !() __attribute__((always_inline)) {
      toggle();
    }

    /*----- comptabled with DigitalPin ----------*/
    inline __attribute__((always_inline))
    void toggle() {
      digitalToggle(CPin);
    }

    inline __attribute__((always_inline))
    void config(uint32_t mode, bool value) {  /*compatale with digitalPin*/
      this->mode(mode);
      this->write(value);
    }
    inline __attribute__((always_inline))
    void mode(uint32_t mode) {
      pinMode(CPin, mode);
    }
    inline __attribute__((always_inline))
    void attach( void (*callback)(void), uint32_t mode) {
      attachInterrupt(CPin.ucPin, callback, mode);
    }

    inline __attribute__((always_inline))
    void detach(void) {
      detachInterrupt(CPin.ucPin);
    }
};

class InputPin : public LL_PIN {
  public:
    constexpr InputPin(__ConstPin CPin, bool initial_value = 1): LL_PIN(CPin) {
      config(INPUT, initial_value);
    }

    template<typename T = bool>
    inline operator T () {
      return read();
    }

    uint32_t pulseIn(bool state = false, uint32_t timeout = 1'000'000L )
    {
      // Cache the port and bit of the pin in order to speed up the
      // pulse width measuring loop and achieve finer resolution.
      // Calling digitalRead() instead yields much coarser resolution.
      uint32_t startMicros = micros();

      // wait for any previous pulse to end
      while (read() == state) {
        if (micros() - startMicros > timeout)
          return 0;
      }

      // wait for the pulse to start
      while (read() != state) {
        if (micros() - startMicros > timeout)
          return 0;
      }

      uint32_t start = micros();
      // wait for the pulse to stop
      while (read() == state) {
        if (micros() - startMicros > timeout)
          return 0;
      }
      return (micros() - start);
    }
};

class OutputPin : public LL_PIN {
  public:
    constexpr OutputPin(__ConstPin CPin, bool initial_value = 0): LL_PIN(CPin) {
      config(OUTPUT, initial_value);
    }

    void pulse(uint32_t delaycnt = F_CPU / 10'000'000, bool value = true) {
      this->write(value);
      if (delaycnt) {
        for (volatile uint32_t i = delaycnt; i > 0; i--);
      }
      this->toggle();
    }
    template<typename T = bool>
    inline operator T () {
      return read();
    }

    inline void operator  !() __attribute__((always_inline)) {
      toggle();
    }

    template<typename T>
    inline OutputPin & operator = (T value) {
      write(value);
      return *this;
    }

    template<typename T>
    inline OutputPin & operator ^= (T value) {
      if(value) toggle();
      return *this;
    }
};

template < uint8_t nbits = 8, uint8_t bit_order = MSBFIRST, uint32_t delaycnt = F_CPU / 10'000'000 >
class ClockedInput {
    // A DirectIO implementation of shiftIn. Also supports
    // a variable number of bits (1-32); shiftIn is always 8 bits.
  public:
    // Define a type large enough to hold nbits bits (see base.h)
    typedef bits_type(nbits) bits_t;
    constexpr ClockedInput(__ConstPin data_pin, __ConstPin clock_pin , bool pullup = true) : data(data_pin, pullup), clock(clock_pin) {}
    InputPin data;
    OutputPin clock;

    bits_t read() {
      // read nbits bits from the input pin and pack them
      // into a value of type bits_t.

      bits_t value = 0;
      bits_t mask = (bit_order == LSBFIRST) ? 1 : (bits_t(1) << (nbits - 1));

      for (uint8_t i = 0; i < nbits; i++) {
        clock = HIGH;
        if (delaycnt) {
          for (volatile uint32_t i = delaycnt; i > 0; i--);
        }

        if (data) {
          value |= mask;
        }
        clock = LOW;

        if (bit_order == LSBFIRST) {
          mask <<= 1;
        }
        else {
          mask >>= 1;
        }
        if (delaycnt) {
          for (volatile uint32_t i = delaycnt; i > 0; i--);
        }
      }
      return value;
    }

    operator bits_t() {
      return read();
    }
};

template < uint8_t nbits = 8, uint8_t bit_order = MSBFIRST, uint32_t delaycnt = F_CPU / 10'000'000 >
class ClockedOutput {
    // A DirectIO implementation of shiftOut. Also supports
    // a variable number of bits (1-32); shiftOut is always 8 bits.
  public:
    // Define a type large enough to hold nbits bits (see base.h)
    typedef bits_type(nbits) bits_t;

    constexpr ClockedOutput(__ConstPin data_pin, __ConstPin clock_pin): data(data_pin), clock(clock_pin) {};
    OutputPin data;
    OutputPin clock;

    void write(bits_t val) {
      // write nbits bits to the output pin
      bits_t mask = (bit_order == LSBFIRST) ? 1 : (bits_t(1) << (nbits - 1));

      for (uint8_t i = 0; i < nbits; i++) {
        data = (val & mask);
        clock.pulse(delaycnt);

        if (bit_order == LSBFIRST) {
          mask <<= 1;
        }
        else {
          mask >>= 1;
        }
      }
    }

    ClockedOutput& operator = (bits_t val) {
      write(val);
      return *this;
    }
};

#define GPIOPIN LL_PIN
// This macro lets you temporarily set an output to a value, 
// and toggling back at the end of the code block. For example:
// 
// Output<2> cs;
// Output<3> data;
// with(cs, LOW) {
//     data = HIGH;
// }
// 
// is equivalent to:
// cs = LOW;
// data = HIGH;
// cs = HIGH;

#define with(pin, val) for(boolean _loop_##pin=((pin=val),true);_loop_##pin; _loop_##pin=((pin=!val), false))

#endif /*__cplusplus*/
#endif /*__EFM32GPIO_H__*/
