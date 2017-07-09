//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2009-2012 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 9453 of the EK-LM4F232 Firmware Package.
// LM3S811 for arduino                   by huaweiwx@sina.com 2015.7
//*****************************************************************************

#include "arduino.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);
extern void GPIOAIntHandler(void);
extern void GPIOBIntHandler(void);
extern void GPIOCIntHandler(void);
extern void GPIODIntHandler(void);
extern void GPIOEIntHandler(void);
extern void SysTickIntHandler(void);
/*
 * create some overridable default signal handlers
 */
__attribute__((weak)) void UARTIntHandler(void) {}
__attribute__((weak)) void UARTIntHandler1(void) {}
__attribute__((weak)) void I2CIntHandler(void) {}

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
//static unsigned long pulStack[128];
extern unsigned _estack;
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
//    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
	(void *)&_estack, 
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    SysTickIntHandler,                      // The SysTick handler
    GPIOAIntHandler,                        // GPIO Port A
    GPIOBIntHandler,                        // GPIO Port B
    GPIOCIntHandler,                        // GPIO Port C
    GPIODIntHandler,                        // GPIO Port D
    GPIOEIntHandler,                        // GPIO Port E
    UARTIntHandler,                         // UART0 Rx and Tx
    UARTIntHandler1,                        // UART1 Rx and Tx
    IntDefaultHandler,                      // SSI0 Rx and Tx
    I2CIntHandler,                          // I2C0 Master and Slave
    IntDefaultHandler,                      // PWM Fault
    IntDefaultHandler,                      // PWM Generator 0
    IntDefaultHandler,                      // PWM Generator 1
    IntDefaultHandler,                      // PWM Generator 2
    IntDefaultHandler,                      // Quadrature Encoder 0
    IntDefaultHandler,                      // ADC Sequence 0
    IntDefaultHandler,                      // ADC Sequence 1
    IntDefaultHandler,                      // ADC Sequence 2
    IntDefaultHandler,                      // ADC Sequence 3
    IntDefaultHandler,                      // Watchdog timer
    IntDefaultHandler,                      // Timer 0 subtimer A
    IntDefaultHandler,                      // Timer 0 subtimer B
    IntDefaultHandler,                      // Timer 1 subtimer A
    IntDefaultHandler,                      // Timer 1 subtimer B
    IntDefaultHandler,                      // Timer 2 subtimer A
    IntDefaultHandler,                      // Timer 2 subtimer B
    IntDefaultHandler,                      // Analog Comparator 0
    IntDefaultHandler,                      // Analog Comparator 1
    IntDefaultHandler,                      // Analog Comparator 2
    IntDefaultHandler,                      // System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // FLASH Control
 };

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
//add hw
extern void (*__preinit_array_start[])(void);
extern void (*__preinit_array_end[])(void);
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);
extern void init(void);
//add hw
//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;
    unsigned i, cnt;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    //
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //
    // HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         // ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        // NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    //
    // Call the application's entry point.
	
	    //
    // call any global c++ ctors
    //
    cnt = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < cnt; i++)
        __preinit_array_start[i]();

    init();

    cnt = __init_array_end - __init_array_start;
    for (i = 0; i < cnt; i++)
        __init_array_start[i]();
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}

/* syscall stuff */
//void *__dso_handle = 0;

/**
 * _sbrk - newlib memory allocation routine
 */
typedef char *caddr_t;

caddr_t _sbrk (int incr)
{
    double current_sp;
    extern char end asm ("end"); /* Defined by linker */
    static char * heap_end;
    char * prev_heap_end;

    if (heap_end == NULL) {
        heap_end = &end; /* first ram address after bss and data */
    }

    prev_heap_end = heap_end;

    // simplistic approach to prevent the heap from corrupting the stack
    // TBD: review for alternatives
    if ( heap_end + incr < (caddr_t)&current_sp ) {
        heap_end += incr;
        return (caddr_t) prev_heap_end;
    }
    else {
        return NULL;
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__((weak))
extern int link( char *cOld, char *cNew )
{
    return -1 ;
}

__attribute__((weak))
extern int _close( int file )
{
    return -1 ;
}

__attribute__((weak))
extern int _fstat( int file, struct stat *st )
{
    st->st_mode = S_IFCHR ;

    return 0 ;
}

__attribute__((weak))
extern int _isatty( int file )
{
    return 1 ;
}

__attribute__((weak))
extern int _lseek( int file, int ptr, int dir )
{
    return 0 ;
}

__attribute__((weak))
extern int _read(int file, char *ptr, int len)
{
    return 0 ;
}

__attribute__((weak))
extern int _write( int file, char *ptr, int len )
{
    return len;
}

__attribute__((weak))
extern void _kill( int pid, int sig )
{
    return ;
}

__attribute__((weak))
extern int _getpid ( void )
{
    return -1 ;
}

/*
__attribute__((weak))
extern void _exit (void)
{

}
*/