

/*GCC tools chain check  add by huaweiwx@sina.com 2018.9.7*/
#if __GNUC__ > 5 || (__GNUC__ == 5 && (__GNUC_MINOR__ > 4 || \
   (__GNUC_MINOR__ == 4 && __GNUC_PATCHLEVEL__ > 0)))

#include <Arduino.h>

#if defined(PART_LM3S811)
//    #include "inc/lm3S811.h"
#elif defined(PART_LM3S8962)
//  #include "inc/lm3s8962.h"
#else
#error "**** No PART defined or unsupported PART ****"
#endif

#include "inc/hw_gpio.h"


//#include "eeprom.h"

#ifdef __cplusplus
extern "C" {

void init(void)
{
	timerInit(); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
#if defined(PART_LM3S8962)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);	
#endif	
} /* void _init(void) */

} /* extern "C" */
#endif

int main(void)
{
	
#if defined(PART_LM3S8962)
	init();
#endif
	setup();

	for (;;) {
		loop();
	}
}


#else
#error "Please update to GCC ver 5-2016q2 https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads"	
#endif
