/***********************************************************************
 *	Constpin.h
  Copyright (c) 2018  HuaWei huaweiwx@sina.com
  
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
 ***********************************************************************/
#ifndef _CONSTPIN_H_
#define _CONSTPIN_H_

#if defined(__LM3S811__)
#define CHIP_PIN_LIST \
	PIN(A,0),PIN(A,1),PIN(A,2),PIN(A,3),PIN(A,4),PIN(A,5),\
	PIN(B,0),PIN(B,1),PIN(B,2),PIN(B,3),PIN(B,4),PIN(B,5),PIN(B,6), \
										PIN(C,4),PIN(C,5),PIN(C,6),PIN(C,7),\
	PIN(D,0),PIN(D,1),PIN(D,2),PIN(D,3),PIN(D,4),PIN(D,5),PIN(D,6),PIN(D,7),\
	PIN(E,0),PIN(E,1),

#elif defined(__LM3S8962__)
#define CHIP_PIN_LIST \
	PIN(A,0),PIN(A,1),PIN(A,2),PIN(A,3),PIN(A,4),PIN(A,5),PIN(A,6),PIN(A,7),\
	PIN(B,0),PIN(B,1),PIN(B,2),PIN(B,3),PIN(B,4),PIN(B,5),PIN(B,6),         \
										PIN(C,4),PIN(C,5),PIN(C,6),PIN(C,7), \
	PIN(D,0),PIN(D,1),PIN(D,2),PIN(D,3),PIN(D,4),PIN(D,5),PIN(D,6),PIN(D,7), \
	PIN(E,0),PIN(E,1),PIN(E,2),PIN(E,3),\
	PIN(F,0),PIN(F,1),PIN(F,2),PIN(F,3),\
	PIN(G,0),PIN(G,1),

#error  !this mcu unsupported!
#endif 





#endif
