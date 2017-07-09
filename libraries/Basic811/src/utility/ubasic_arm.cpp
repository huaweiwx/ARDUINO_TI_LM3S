/*                  2014, huaweiwx@wxmedi.com
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. Neither the name of the author nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.
*/
//ver hw150804 for arduino

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>    /* printf() */
#include <stdlib.h>   /* exit() */
#include "arduino.h"
#include "mytypedef.h"
#include "tokenizer.h"
#include "mybasic.h"
#include "basic_prog.h"
#include "ubasic.h"
#include <stdbool.h>

/* void arduino_prints(char * s);
  void arduino_printi(int32_t i); */
//----------------------------------------------------------------------------------------------------
extern      sGLOBEL_T BasProg;
extern void eepromWrite8(uint16_t adr, uint8_t d);

static void
set_statement(void)
{
  uint8_t id;
  int val;
  accept(TOKENIZER_SET);
  id = (uint8_t) expr();
  switch (id) {
#ifdef AUTO_RUNADR
  case 1:     //auto run
	  if (tokenizer_token()==TOKENIZER_CR) {   //not arg1 default 10 
        accept(TOKENIZER_CR);
	  }else{
		if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next();   //skip ','
        id = expr();
        accept(TOKENIZER_CR);
      };		
      id = (id < 3)? id:0;  //0 off,1 ram,2 eep,3 com,4 flash
      eepromWrite8(AUTO_RUNADR,id);							
      break;	
	case 2:     //auto run eep
	  if (tokenizer_token()==TOKENIZER_CR) {   //not arg1 default 10 
	    id = 0;
        accept(TOKENIZER_CR);
	  }else{
		if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next();   //skip ','
        id = expr();
        accept(TOKENIZER_CR);
      };		
      id = (id < 5)? id:0;  //0 off,1 ram,2 eep,3 com,4 flash
      eepromWrite8(AUTO_RUNADR+1,id);							
      break;
#endif	  
    case 10:    //显示格式 B H D
	  if (tokenizer_token()==TOKENIZER_CR) {   //not arg1 default 10 
        accept(TOKENIZER_CR);
	  }else{
		if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next();   //skip ','
        id = expr();
        accept(TOKENIZER_CR);
      };		
      BasProg.print_base = ((id ==  2)? 2:
	                        (id == 16)?16:10);
      break;
    /* 	case 1:
          accept(TOKENIZER_CR);
          arduino_prints("set\r\n");   */
    default:
      break;
  };
argc1:
{};
argc2:
{};
ERR:
{
	
};
}

static void
sys_statement(void)
{
  uint8_t id;
  int val;
  accept(TOKENIZER_SYS);
  id = (uint8_t) expr();
  switch (id) {
#if defined(__AVR__)
	case 0:
      asm volatile ("jmp 0");
      break;
#endif	  
#if defined(EE_PROG_START_ADR)
    case 1:  
      accept(TOKENIZER_CR);
	  val = EE_PROG_START_ADR;
      EE2PROM.write(val, 0xff);
      OUTDEV.println(F("format eedisk"));
	  break;
#endif	  
    default:
      break;
  };
argc1:
{};
argc2:
{};
ERR:
{
	
};
}

int statement_hack(int tk) {
  switch (tk) {
    case TOKENIZER_SYS:
      sys_statement();
      return true;
    case TOKENIZER_SET:
      set_statement();
      return true;
    default:
      break;
  }
  return false;
}

//info()
uint32_t get_sysinfo(uint8_t id) {   
  uint8_t p;
  switch (id)
  {
    case 0:
      return  ARDUINO_BASIC_VAR;
#ifdef 	 NUM_DIGITAL_PINS
    case 1:
      return  NUM_DIGITAL_PINS;
#endif
#ifdef  NUM_ANALOG_INPUTS
    case 2:
      return  NUM_ANALOG_INPUTS;
#endif
     case 3:
      return BasProg.lenRam;  //free lenth  
#if  EE2LENTH
     case 4:
       return EE2LENTH;  //ee2prom lenth
#endif	   
#if  EE1LENTH
     case 5:
       return EE1LENTH;  //ee2prom lenth
#endif
#if  EE_DATA_RESEVE
     case 6:
       return EE_DATA_RESEVE;  //ee2prom lenth
#endif	   
#if  EE_DATA_USER
     case 7:
       return EE_DATA_USER/2;  //ee2prom lenth
#endif	   
	   

    /*     case 9:
    	    return API_SysStruct.Sec;
    */
    //    case 10:
    //	    p = expr();
    //	    return  (uint32_t)digitalPinToPort(p);
    //     case 11:
    //	    p = expr();
    //	    return  digitalPinToBitMask(p);
    //	 case 12:
    //	    p = expr();
    //	    return  digitalPinToTimer(p);
    //     case 13:
    //	    p = expr();
    //        return analogInPinToBit(p);
    /*     case 14:
    	    p = expr();
            return analogInPinToBit(p);
    */

    case 40:
      return MAX_PROG_RAM;
    case 41:
      return sizeof(PROG_RAM_DEFAULT);
    case 42:
      return (MAX_PROG_RAM - (sizeof(PROG_RAM_DEFAULT)));
  }
  //	 printf("Err:Info %d\n",Args.uni.u8a[1]);
  return 0;
}

/*------------------function_hack------------------------*/
int function_hack(uint8_t tk, int32_t *r) {
  uint8_t id;
//  SERx.println(F("func hack"));
  switch (tk)
  {
    default:
      return false;
      break;
  };
  return true;
}

