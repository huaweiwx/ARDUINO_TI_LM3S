#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#ifndef BV
#define BV(x) (1 << (x))
#endif

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/adc.h"
#include "variant.h"
#include "chip_pindef.h"

#ifdef VARIANT_PIN_LIST
	#define PIN_LIST VARIANT_PIN_LIST
#else
	#define PIN_LIST CHIP_PIN_LIST
#include "variant_default.h"
#endif

#ifdef __cplusplus

class __ConstPin {
  public:
    constexpr __ConstPin(const uint32_t port, const  uint32_t pin, const uint8_t val)
      : port(port), pin(pin), val(val) {};
    constexpr operator int() const {
      return val;
    }
    const uint32_t port;
    const  uint32_t pin;
    const uint8_t val;
};
#define PIN(a, b) __P##a##b
enum {
  PIN_LIST,
  NUM_DIGITAL_PINS,
};
#undef PIN

/*Compatible with stm32 arduino core*/
#define PIN(a, b)  P##a##b((const uint32_t)GPIO_PORT ## a ##_BASE, BV(b), __P##a##b)
constexpr __ConstPin PIN_LIST;
#undef PIN

/*Compatible with Nuvoton arduino core*/
#define PIN(a, b)  P##a##_##b((const uint32_t)GPIO_PORT ## a ##_BASE, BV(b), __P##a##b)
constexpr __ConstPin PIN_LIST;
#undef PIN

#define ARDUINOPIN_TypeDef 	__ConstPin

#else
#define ARDUINOPIN_TypeDef 	uint8_t

/*Compatible with stm32 arduino core*/
#define PIN(a, b) P##a##b
enum {
  PIN_LIST,
  NUM_DIGITAL_PINS,
};
#undef PIN

/*Compatible with Nuvoton arduino core*/
#define PIN(a, b) P##a##_##b
enum {
  PIN_LIST
};
#undef PIN

#endif

#include "lm3sgpio.h"
#endif
