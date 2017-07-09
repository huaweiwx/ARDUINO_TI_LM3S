//*****************************************************************************
//
// rit128x96x4.h - Prototypes for the driver for the RITEK 128x96x4 graphical
//                   OLED display.
//
// Copyright (c) 2007-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 10636 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#ifndef __RIT128X96X4_H__
#define __RIT128X96X4_H__
#define LINE_HIGH     10
#define LINE_WIDTCH    6
//*****************************************************************************
//
// Prototypes for the driver APIs.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"{ 
#endif 

extern void RIT128x96x4Clear(void);
extern void RIT128x96x4StringDraw(const char *pcStr,
                                    unsigned long ulX,
                                    unsigned long ulY,
                                    unsigned char ucLevel);
extern void RIT128x96x4ImageDraw(const unsigned char *pucImage,
                                   unsigned long ulX,
                                   unsigned long ulY,
                                   unsigned long ulWidth,
                                   unsigned long ulHeight);
extern void RIT128x96x4Init(unsigned long ulFrequency);
extern void RIT128x96x4Enable(unsigned long ulFrequency);
extern void RIT128x96x4Disable(void);
extern void RIT128x96x4DisplayOn(void);
extern void RIT128x96x4DisplayOff(void);

#ifdef __cplusplus
} //extern "C"{ 
#include <print.h>

class DISP: public Print{
   public:
    DISP(){
		RIT128x96x4Init(1000000);
		begin();
		};
	
	void begin(uint8_t cols, uint8_t lines, uint8_t dotsize=LINE_WIDTCH);
	void begin(void);
	
	
    void clrScr(void);
	
	void setXY(uint8_t x, uint8_t y, uint8_t level=15);
	
	void setCursor(uint8_t l, uint8_t c, uint8_t level=15);
	
	void home(void){
		this->setCursor(0, 0, 15);	
	}
	void setContrast(uint8_t l){
		this->Level = l;
	}
	
    void StringDraw(const char *pcStr,
                    unsigned long ulX,
                    unsigned long ulY,
                    unsigned char ucLevel)
	{
		RIT128x96x4StringDraw(pcStr,ulX,ulY,ucLevel); 
	};
	
    void ImageDraw(const unsigned char *pucImage,
                              unsigned long ulX,
                              unsigned long ulY,
                              unsigned long ulWidth,
                              unsigned long ulHeight)
	{
		RIT128x96x4ImageDraw(pucImage,ulX,ulY,ulWidth,ulHeight);						  
	};
	void drawPixel(unsigned long ulX,unsigned long ulY);
    void drawHLine(int x, int y, int l);
    void drawVLine(int x, int y, int l);
	void drawLine(int x1, int y1, int x2, int y2);
	
	void Enable(unsigned long ulFrequency)
	{
		RIT128x96x4Enable(ulFrequency);
	};
	void Disable(void){RIT128x96x4Disable();};
	void lcdOn(void){RIT128x96x4DisplayOn();};
	void lcdOff(void){RIT128x96x4DisplayOff();};
    virtual size_t write(uint8_t);
    using Print::write;
   private:
   int8_t 
     unsigned long lX,lY;
     unsigned char Level,cols,lines,dotsize;
}; 
#endif 

#endif // __RIT128X96X4_H__
