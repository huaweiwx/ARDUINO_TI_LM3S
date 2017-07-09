/*
 ************************************************************************
 *	HardwareSerial.cpp
 *
 *	Arduino core files for ARM Cortex-M3: LM3S Stellaris
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *
 ***********************************************************************

 2013-12-23 Limited size for RX and TX buffers, by spirilis

 
  Derived from:
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring_private.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "inc/gpio.h"
#include "inc/debug.h"
#include "inc/interrupt.h"
#include "inc/pin_map.h"
#include "inc/sysctl.h"
#include "inc/uart.h"

#include "part.h"	
#include "HardwareSerial.h"

#define TX_BUFFER_EMPTY    (txReadIndex == txWriteIndex)
#define TX_BUFFER_FULL     (((txWriteIndex + 1) % txBufferSize) == txReadIndex)

#define RX_BUFFER_EMPTY    (rxReadIndex == rxWriteIndex)
#define RX_BUFFER_FULL     (((rxWriteIndex + 1) % rxBufferSize) == rxReadIndex)

#define UART_BASE g_ulUARTBase[uartModule]

static const unsigned long g_ulUARTBase[2] =
{
    UART0_BASE, UART1_BASE
};

//*****************************************************************************
//
// The list of possible interrupts for the console UART.
//
//*****************************************************************************
static const unsigned long g_ulUARTInt[2] =
{
    INT_UART0, INT_UART1
};

//*****************************************************************************
//
// The list of UART peripherals.
//
//*****************************************************************************
static const unsigned long g_ulUARTPeriph[2] =
{
    SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1
};
//*****************************************************************************
//
// The list of UART GPIO configurations.
//
//*****************************************************************************
 static const unsigned long g_ulUARTConfig[2][2] =
 {
   {U0RX_PIN, U0TX_PIN}, {U1RX_PIN, U1TX_PIN}
 };
 
static const unsigned long g_ulUARTPort[2] =
{
	GPIO_PORTA_BASE,GPIO_PORTD_BASE
};
	


static const unsigned long g_ulUARTPins[2] =
{
    GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_2 | GPIO_PIN_3
};

// Constructors ////////////////////////////////////////////////////////////////
HardwareSerial::HardwareSerial(void)
{
    txWriteIndex = 0;
    txReadIndex = 0;
    rxWriteIndex = 0;
    rxReadIndex = 0;
    uartModule = 0;

    txBuffer = (unsigned char *) 0xFFFFFFFF;
    rxBuffer = (unsigned char *) 0xFFFFFFFF;
    txBufferSize = SERIAL_BUFFER_SIZE;
    rxBufferSize = SERIAL_BUFFER_SIZE;
}

HardwareSerial::HardwareSerial(unsigned long module) 
{
    txWriteIndex = 0;
    txReadIndex = 0;
    rxWriteIndex = 0;
    rxReadIndex = 0;
    uartModule = module;

    txBuffer = (unsigned char *) 0xFFFFFFFF;
    rxBuffer = (unsigned char *) 0xFFFFFFFF;
    txBufferSize = SERIAL_BUFFER_SIZE;
    rxBufferSize = SERIAL_BUFFER_SIZE;
}
// Private Methods //////////////////////////////////////////////////////////////
void
HardwareSerial::flushAll(void)
{
    // wait for transmission of outgoing data
    while(!TX_BUFFER_EMPTY)
    {
    }
    while (UARTBusy(UART_BASE)) ;
    txReadIndex = 0;
    txWriteIndex = 0;

    //
    // Flush the receive buffer.
    //
    rxReadIndex = 0;
    rxWriteIndex = 0;
}

void
HardwareSerial::primeTransmit(unsigned long ulBase)
{
    //
    // Do we have any data to transmit?
    //
    if(!TX_BUFFER_EMPTY)
    {
        //
        // Disable the UART interrupt. If we don't do this there is a race
        // condition which can cause the read index to be corrupted.
        //
        IntDisable(g_ulUARTInt[uartModule]);
        //
        // Yes - take some characters out of the transmit buffer and feed
        // them to the UART transmit FIFO.
        //
        while(!TX_BUFFER_EMPTY)
        {
            while(UARTSpaceAvail(ulBase) && !TX_BUFFER_EMPTY){
                UARTCharPutNonBlocking(ulBase,
                                       txBuffer[txReadIndex]);

                txReadIndex = (txReadIndex + 1) % txBufferSize;
            }
        }

        //
        // Reenable the UART interrupt.
        //
        IntEnable(g_ulUARTInt[uartModule]);
    }
}

// Public Methods //////////////////////////////////////////////////////////////

void
HardwareSerial::begin(unsigned long baud)
{
	baudRate = baud;
    //
    // Initialize the UART.
    //
    SysCtlPeripheralEnable(g_ulUARTPeriph[uartModule]);

    //TODO:Add functionality for PinConfigure with variable uartModule
    GPIOPinConfigure(g_ulUARTConfig[uartModule][0]);
    GPIOPinConfigure(g_ulUARTConfig[uartModule][1]);

    GPIOPinTypeUART(g_ulUARTPort[uartModule], g_ulUARTPins[uartModule]);

    UARTConfigSetExpClk(UART_BASE, SysCtlClockGet(), baudRate,
                            (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_WLEN_8));
    //
    // Set the UART to interrupt whenever the TX FIFO is almost empty or
    // when any character is received.
    //
    UARTFIFOLevelSet(UART_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    flushAll();
    UARTIntDisable(UART_BASE, 0xFFFFFFFF);
    UARTIntEnable(UART_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(g_ulUARTInt[uartModule]);

    //
    // Enable the UART operation.
    //
    UARTEnable(UART_BASE);

    // Allocate TX & RX buffers
    if (txBuffer != (unsigned char *)0xFFFFFFFF)  // Catch attempts to re-init this Serial instance by freeing old buffer first
        free(txBuffer);
    if (rxBuffer != (unsigned char *)0xFFFFFFFF)  // Catch attempts to re-init this Serial instance by freeing old buffer first
        free(rxBuffer);
    txBuffer = (unsigned char *) malloc(txBufferSize);
    rxBuffer = (unsigned char *) malloc(rxBufferSize);

    SysCtlDelay(100);
}

void
HardwareSerial::setBufferSize(unsigned long txsize, unsigned long rxsize)
{
    if (txsize > 0)
        txBufferSize = txsize;
    if (rxsize > 0)
        rxBufferSize = rxsize;
}

void
HardwareSerial::setModule(unsigned long module)
{
    UARTIntDisable(UART_BASE, UART_INT_RX | UART_INT_RT);
    IntDisable(g_ulUARTInt[uartModule]);
	uartModule = module;
	begin(baudRate);

}
/*
void 
HardwareSerial::setPins(unsigned long pins)
{
	if(pins == UART1_PORTA)
	{
		//Default UART0Pin Muxing
		GPIOPinConfigure(g_ulUARTConfig[0][0]);
		GPIOPinConfigure(g_ulUARTConfig[0][1]);
		GPIOPinTypeUART(g_ulUARTPort[0], g_ulUARTPins[0]);
	}
	else  //PORTD
	{
		GPIOPinConfigure(g_ulUARTConfig[1][0]);
		GPIOPinConfigure(g_ulUARTConfig[1][1]);
		GPIOPinTypeUART(g_ulUARTPort[1], g_ulUARTPins[1]);
	}
}
*/
HardwareSerial::operator bool()
{
	return true;
}

void HardwareSerial::end()
{
    unsigned long ulInt = IntMasterDisable();

	flushAll();

    //
    // If interrupts were enabled when we turned them off, turn them
    // back on again.
    //
    if(!ulInt)
    {
        IntMasterEnable();
    }

    IntDisable(g_ulUARTInt[uartModule]);
    UARTIntDisable(UART_BASE, UART_INT_RX | UART_INT_RT);
}

int HardwareSerial::available(void)
{
    return((rxWriteIndex >= rxReadIndex) ?
		(rxWriteIndex - rxReadIndex) : rxBufferSize - (rxReadIndex - rxWriteIndex));
}

int HardwareSerial::peek(void)
{
    unsigned char cChar = 0;

    //
    // Wait for a character to be received.
    //
    if(RX_BUFFER_EMPTY)
    {
    	return -1;
    	//
        // Block waiting for a character to be received (if the buffer is
        // currently empty).
        //
    }

    //
    // Read a character from the buffer.
    //
    cChar = rxBuffer[rxReadIndex];
    //
    // Return the character to the caller.
    //
    return(cChar);
}

int HardwareSerial::read(void)
{
    if(RX_BUFFER_EMPTY) {
    	return -1;
    }

    //
    // Read a character from the buffer.
    //
    unsigned char cChar = rxBuffer[rxReadIndex];
	rxReadIndex = ((rxReadIndex) + 1) % rxBufferSize;
	return cChar;
}

void HardwareSerial::flush()
{
    while(!TX_BUFFER_EMPTY);
    while (UARTBusy(UART_BASE)) ;
}

size_t HardwareSerial::write(uint8_t c)
{

    unsigned int numTransmit = 0;
    //
    // Check for valid arguments.
    //
//    ASSERT(c != 0);
/*
    //this is not necessary: https://github.com/energia/Energia/issues/225
    //
    // If the character to the UART is \n, then add a \r before it so that
    // \n is translated to \n\r in the output.
    //

	// If the output buffer is full, there's nothing for it other than to
	// wait for the interrupt handler to empty it a bit

    if(c == '\n')
    {
        while (TX_BUFFER_FULL);
        txBuffer[txWriteIndex] = '\r';
		txWriteIndex = (txWriteIndex + 1) % txBufferSize;
        numTransmit ++;
    }
*/
    //
    // Send the character to the UART output.
    //
    while (TX_BUFFER_FULL);
    txBuffer[txWriteIndex] = c;
    txWriteIndex = (txWriteIndex + 1) % txBufferSize;
    numTransmit ++;

    //
    // If we have anything in the buffer, make sure that the UART is set
    // up to transmit it.
    //
    if(!TX_BUFFER_EMPTY)
    {
	    primeTransmit(UART_BASE);
        UARTIntEnable(UART_BASE, UART_INT_TX);
    }

    //
    // Return the number of characters written.
    //
    return(numTransmit);
}

void HardwareSerial::UARTIntHandler(void){
    unsigned long ulInts;
    long lChar;
    // Get and clear the current interrupt source(s)
    //
    ulInts = UARTIntStatus(UART_BASE, true);
    UARTIntClear(UART_BASE, ulInts);

    // Are we being interrupted because the TX FIFO has space available?
    //
    if(ulInts & UART_INT_TX)
    {
        //
        // Move as many bytes as we can into the transmit FIFO.
        //
        primeTransmit(UART_BASE);

        //
        // If the output buffer is empty, turn off the transmit interrupt.
        //
        if(TX_BUFFER_EMPTY)
        {
            UARTIntDisable(UART_BASE, UART_INT_TX);
        }
    }
    if(ulInts & (UART_INT_RX | UART_INT_RT))
    {
        while(UARTCharsAvail(UART_BASE))
            {

            //
            // Read a character
            //
            lChar = UARTCharGetNonBlocking(UART_BASE);
            //
            // If there is space in the receive buffer, put the character
            // there, otherwise throw it away.
            //
            uint8_t volatile full = RX_BUFFER_FULL;
            if(full) break;

            rxBuffer[rxWriteIndex] =
                (unsigned char)(lChar & 0xFF);
            rxWriteIndex = ((rxWriteIndex) + 1) % rxBufferSize;

            //
            // If we wrote anything to the transmit buffer, make sure it actually
            // gets transmitted.
            //
        }
        primeTransmit(UART_BASE);
        UARTIntEnable(UART_BASE, UART_INT_TX);
    }
}

void
UARTIntHandler(void)
{
    Serial.UARTIntHandler();
}

void
UARTIntHandler1(void)
{
    Serial1.UARTIntHandler();
}
HardwareSerial Serial;
HardwareSerial Serial1(1);
