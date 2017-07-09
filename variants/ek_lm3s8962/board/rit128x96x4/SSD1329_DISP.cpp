//*****************************************************************************
//
// rit128x96x4.c - Driver for the RIT 128x96x4 graphical OLED display.
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

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************
#include "rit128x96x4.h"


void DISP::begin(uint8_t cols, uint8_t lines, uint8_t dotsize){
	this->cols    = cols;
	this->lines   = lines;
	this->dotsize = dotsize;
	this->lX = 0;
	this->lY = 0;
	this->Level = 12;
}


void DISP::begin(void){
	this->begin((uint8_t)128,(uint8_t)96,(uint8_t)6);
}

void DISP::clrScr(void){
		 RIT128x96x4Clear();
		 this->home();
	};

void DISP::setXY(uint8_t x, uint8_t y, uint8_t level){
	     this->lX = x;
		 this->lY = y; 
		 this->Level =level;
};

void DISP::setCursor(uint8_t l, uint8_t c, uint8_t level){
	     this->lX = c * this->dotsize;
		 this->lY = l * LINE_HIGH +3; 
		 this->Level =level;
	};

void DISP::drawPixel(unsigned long ulX,unsigned long ulY){
	unsigned char ch[1];
    ulX &=0xFe;	
    ch[0]= this->Level;
    ch[0] |=  4<<((this->Level)&0x0f);
	RIT128x96x4ImageDraw(ch,ulX,ulY,2,1);
}

void DISP::drawHLine(int x, int y, int l)
{
	unsigned char ch[1];
    ch[0]= this->Level;
    ch[0] |=  4<<((this->Level)&0x0f);
	if (l<0)
	{
		x -= l;
		l = -l/2;
	}
	for (int i=0; i<l+1;i++)
	{
        RIT128x96x4ImageDraw(ch,(unsigned long) (x+(i*2)),(unsigned long) y,2,1);
	}
}

void DISP::drawVLine(int x, int y, int l)
{
	unsigned char ch[1];
	if (x&1) ch[0]= this->Level;
    else ch[0] =  4<<((this->Level)&0x0f);
	
	x &= 0xfe;		
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	
	for (int i=0; i<l+1; i++)
	{
        RIT128x96x4ImageDraw(ch,(unsigned long)x,(unsigned long)(y+i),2,1);
	}
}

void DISP::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
                drawPixel((unsigned long) col,(unsigned long) row);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
                drawPixel((unsigned long) col,(unsigned long) row);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
	}
}

size_t DISP::write(uint8_t c){
		char  str[2] ={c,0};
		if(c == '\n'){
			this->lX = 0;
			this->lY += LINE_HIGH;
			return 1;
		}
		RIT128x96x4StringDraw((const char *)str,this->lX,this->lY,this->Level);
		this->lX += this->dotsize;
		if(this->lX > 120) {
			this->lX = 0;
			this->lY += LINE_HIGH;
		};
		return 1;
};


