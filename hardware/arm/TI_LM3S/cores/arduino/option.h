#ifndef __OPTION_H__
#define __OPTION_H__
#define  ARDUINO_EXTEND 1
#if __has_include("HAL_Conf.h")
# include "HAL_Conf.h"
#elif __has_include("configs/HAL_Conf.h")
# include "configs/HAL_Conf.h"
#endif

#endif /*__OPTION_H__*/
