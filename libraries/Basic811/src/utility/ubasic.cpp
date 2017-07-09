/* Copyright (c) 2006, Adam Dunkels
                 2013  huawei huaweiwx@wxmedi.com
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
#define _UBASIC_MAIN_
#if ARDUINO
#include "arduino.h"
#else
#include <stdint.h>
//#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>	
#endif



#include "mytypedef.h"
#include "tokenizer.h"
#include "mybasic.h"
#include "basic_prog.h"
#include "ubasic.h"

#ifdef LCD
  LiquidCrystal lcd(49,51,50, 57,56,55,54);//4 bits drived
#endif

#ifndef IO_REG_TYPE
#define IO_REG_TYPE uint8_t
#endif

#ifndef F    //for arm
#define F(x) (x)
#endif


//globel vars & define
/*---------------------------------------------*/
uint8_t  prog_ram[MAX_PROG_RAM] = PROG_RAM_DEFAULT;
sGLOBEL_T BasProg;
// #define PROG_LENTH sizeof(PROG_RAM_DEFAULT)
/*----------------------------------------------*/
ArgsTypeDef basicVars;

#ifndef MAX_GOSUB_STACK_DEPTH
#define MAX_GOSUB_STACK_DEPTH 10
#endif
static  char* gosub_stack[MAX_GOSUB_STACK_DEPTH];
static  uint8_t gosub_stack_ptr;


//for next
struct for_state {
  char *pafter;
  uint8_t for_variable;
  uint16_t to;
};

#ifndef MAX_FOR_STACK_DEPTH
#define MAX_FOR_STACK_DEPTH 4
#endif
static struct for_state for_stack[MAX_FOR_STACK_DEPTH];
static uint8_t for_stack_ptr;


EVENTS_StackTypeDef       events_stack;    //[MAX_EVENTS_STACK]
EVENTS_ExtiStackTypeDef   intes_stack;      //
EVENTS_TimeStackTypeDef   timers_stack;    //[MAX_EVENTS_STACK]

#ifndef  MAX_VARNUM
#define  MAX_VARNUM 26
#endif
int32_t variables[MAX_VARNUM];


//int32_t function_calcul(void);
void callback_TimeISR(void);
//int function_hack(uint8_t, int32_t *) __attribute__((weak));
//--------------------------------
#if defined(EE1LENTH)||defined(EE2LENTH)
uint8_t eepromRead8(int32_t adr)
{
#if EE1LENTH	// at24cxx extern i2c eeprom  
 #if EE2LENTH   //avr ee2prom
   return((adr < EE2LENTH)? EE2PROM.read((uint16_t)adr):EE1PROM.read((uint16_t)(adr-EE2LENTH)));
 #else
   return(EE1PROM.read((uint16_t)(adr)));
 #endif	
#else
 #if EE2LENTH
   return(EE2PROM.read((uint16_t)adr));
 #else
   return 0;
 #endif	
#endif
}

void eepromWrite8(uint16_t adr, uint8_t d) {
#if EE1LENTH       // at24cxx extern i2c eeprom  
  #if EE2LENTH     //avr ee2prom
    if(adr < EE2LENTH) EE2PROM.write(adr, d);
	else EE1PROM.write((uint16_t)(adr-EE2LENTH), d);
 #else
    EE1PROM.write((uint16_t)(adr), d);
 #endif
#else
  #if EE2LENTH
    EE2PROM.write(adr, d);
  #endif
#endif	
}

/*----- ee2prom at24cxx rw ----------------------------------------------*/

uint16_t
eepromRead16(uint16_t adr){
  uint16_t v  = eepromRead8(adr++);    
           v += eepromRead8(adr)<<8;
  return v;
}

void
eepromWrite16(uint16_t adr, uint16_t val)
{
  	 eepromWrite8(adr++, val &0xff);
	 eepromWrite8(adr  , val>>8);
}
#endif

/***********************************************************/
static unsigned char breakcheck(void)
{
#if ARDUINO
  if (OUTDEV.available())
    return OUTDEV.read() == KEYDEF_CTRL_C;
  return 0;
#else
  if (kbhit())
    return getch() == KEYDEF_CTRL_C;
  else
    return 0;
#endif
}
/***********************************************************/
static int inchar()
{
#if ARDUINO
  while (1)
  {
    if (OUTDEV.available())
      return OUTDEV.read();
  }
#else
  return getchar();
#endif
}

/***********************************************************/
static int16_t input_int(uint16_t ms)
{
  int16_t a = 0;
#if ARDUINO
  while (1)
  {
    if (OUTDEV.available()) {
      OUTDEV.setTimeout(ms);
      a = OUTDEV.parseInt();
      return a;
    }
  }
#else
  uint8__t ch;
  while (1)
  {
    ch = getchar();
    if (ch == NL) return a;
    if (isdigit(ch))
    {
      a = a * 10 + ch - '0';
    } else {
      return 0;
    }
  }
#endif
}
/***********************************************************/
static void outchar(unsigned char c)
{
#if ARDUINO
  OUTDEV.write(c);
#else
  putchar(c);
#endif
}

/*--------------------------------------------------------------------*/

uint8_t ubasic_finished(void) {
  return GETBIT(BasProg.Flags, GLOBLES_END) || tokenizer_finished();
}


/*---------------------------------------------------------------------------*/
void
ubasic_init(const char *program)
{
  BasProg.pBas = (char *) program;
  for_stack_ptr = gosub_stack_ptr =0; // do_stack_ptr = 0;
//  ubasic_add_init();
  CLRBIT(BasProg.Flags, GLOBLES_END);
  GETBIT(BasProg.Flags, IN_EVENTS_PROC);
  tokenizer_init(program);
}
/*---------------------------------------------------------------------------*/
void
accept(uint8_t token)
{
  if (token != tokenizer_token()) {
    OUTDEV.print(F("acpt"));	  OUTDEV.print(token);
    tokenizer_error_print();
    //    exit(1);
    return;
  }
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/

int16_t isArray(void){  // return array adr
   int16_t adr=-1;
   if(BasProg.pChar[0]=='['){
     tokenizer_next();  //skip '['
     adr = expr();
     accept(TOKENIZER_RIGHTSPAREN);
   }
   return adr;
}

void arrayWrite(uint8_t varnum,uint16_t adr,int16_t v){
	uint16_t i;
	switch (varnum + 'A'){
#if EE_DATA_USER_START    //nether ee2prom nor 24cxx i2c_eeprom 
       case 'E':
	     i = EE_DATA_USER_START+(uint16_t)adr*2;
	     if(adr < (EE_PROG_START_ADR-1)){
	        eepromWrite16(i,v);
		 }
		 break;
#endif
       case 'R':
		 adr = adr*2;
		 if (adr < BasProg.lenRam){
           BasProg.pRam[adr] =  ((uint16_t)v) & 0xff;
		   BasProg.pRam[adr+1] = ((uint16_t)v)>>8 ;
	     }
         break;		  
 	}
}
/*--------------------------------------------------------------------------*/
int32_t arrayRead(uint8_t varnum,uint16_t adr){
	int32_t r=0;
	switch (varnum + 'A'){
#if EE_DATA_USER_START  //nether ee2prom nor 24cxx i2c_eeprom 
       case 'E':
	     adr = EE_DATA_USER_START+(uint16_t)adr*2;
	     if(adr < (EE_PROG_START_ADR-1)) r =(int32_t)eepromRead16(adr);
		  break;
#endif
       case 'R':
	     adr = adr*2;
		 if (adr < BasProg.lenRam)
           r =((uint8_t)BasProg.pRam[adr])+((uint8_t)BasProg.pRam[adr+1])*256;
       break;		  
 	}
  return r;	
}
/*---------------------------------------------------------------------------*/
int32_t factor(void)
{
  uint8_t	varnum;
  int32_t r = 0;
  int16_t adr;
  switch (tokenizer_token()) {
    case TOKENIZER_NUMBER:
      r = tokenizer_num();
      accept(TOKENIZER_NUMBER);
      break;
    case TOKENIZER_HEXNUMBER:
      r = tokenizer_hexnum();
      accept(TOKENIZER_HEXNUMBER);
      break;
    case TOKENIZER_LEFTPAREN:
      accept(TOKENIZER_LEFTPAREN);
      r = expr();
      accept(TOKENIZER_RIGHTPAREN);
      break;
    case TOKENIZER_FUNC:
      r  = function_calcul();
      break;
    case TOKENIZER_VARIABLE:
	  varnum = tokenizer_variable_num();
	  accept(TOKENIZER_VARIABLE);
	  adr = isArray();	 
      if (adr >=0){		 
		r = arrayRead(varnum,adr);		
	  }else{	 
        r = ubasic_get_variable(varnum);
	  } 
      break;
    default:
	   goto ERR;
  }
  return r;
  
ERR:
  BasProg.Err_id = ERR_FACTOR;
  return r;
}
/*---------------------------------------------------------------------------*/
static int32_t
term(void) {
  int32_t f1, f2;
  uint8_t op;
  f1 = factor();
  op = tokenizer_token();
  while (op == TOKENIZER_ASTR ||
         op == TOKENIZER_SLASH ||
         op == TOKENIZER_MOD) {
    tokenizer_next();
    f2 = factor();
    switch (op) {
      case TOKENIZER_ASTR:
        f1 *= f2;
        break;
      case TOKENIZER_SLASH:
        f1 /= f2;
        break;
      case TOKENIZER_MOD:
        f1 %= f2;
        break;
      default:
        BasProg.Err_id = ERR_TERM;
        break;
    }
    op = tokenizer_token();
  }
  return f1;
}
/*---------------------------------------------------------------------------*/
int32_t
expr(void)
{
  int32_t t1, t2;
  uint8_t op;

  t1 = term();
  op = tokenizer_token();
  while (op == TOKENIZER_PLUS ||
         op == TOKENIZER_MINUS ||
         op == TOKENIZER_AND ||
         op == TOKENIZER_OR) {
    tokenizer_next();
    t2 = term();
    switch (op) {
      case TOKENIZER_PLUS:
        t1 += t2;
        break;
      case TOKENIZER_MINUS:
        t1 -= t2;
        break;
      case TOKENIZER_AND:
        t1 &= t2;
        break;
      case TOKENIZER_OR:
        t1 |= t2;
        break;
      default:
        BasProg.Err_id = ERR_EXPR;
        break;
    }
    op = tokenizer_token();
  }
  return t1;
}
/*---------------------------------------------------------------------------*/
static uint8_t
relation(void)
{
  int32_t r1, r2;
  uint8_t op;

  r1 = expr();
  op = tokenizer_token();
  while (op == TOKENIZER_LT ||
         op == TOKENIZER_GT ||
         op == TOKENIZER_EQ) {
    tokenizer_next();
    r2 = expr();
    //    printf("relation: %d %d %d\n", r1, op, r2);
    switch (op) {
      case TOKENIZER_LT:
        r1 = r1 < r2;
        break;
      case TOKENIZER_GT:
        r1 = r1 > r2;
        break;
      case TOKENIZER_EQ:
        r1 = r1 == r2;
        break;
      default:
        BasProg.Err_id = ERR_RELATION;
        break;
    }
    op = tokenizer_token();
  }
  return r1;
}


/*---------------------------------------------------------------------------*/
void ubasic_set_variable(uint8_t varnum, int32_t value) {
  if (varnum >= 0x20 && varnum < (MAX_VARNUM + 0x20)) {   //var
        variables[(varnum - 0x20)] = value;
		return;
     }
  
  switch (varnum + 'A')
  {
#ifdef PORTA
    case 'A':
      PORTA = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTB
    case 'B':
      PORTB = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTC
    case 'C':
      PORTC = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTD
    case 'D':
      PORTD = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTE
    case 'E':
      PORTE = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTF
    case 'F':
      PORTF = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTG
    case 'G':
      PORTG = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTH
    case 'H':
      PORTH =(IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTI
    case 'I':
      PORTI = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTJ
    case 'J':
      PORTJ = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTK
    case 'K':
      PORTK = (IO_REG_TYPE)value;
      break;
#endif
#ifdef PORTL
    case 'L':
      PORTL = (IO_REG_TYPE)value;
      break;
#endif
    case 'S':
      BasProg.Ctrls = (uint16_t)value;
      break;
    case 'T':
      BasProg.Flags = (uint16_t)value;
      break;
    case 'U':
      //		        Tbuf_PutChar((uint8_t)value,&RT_COM1);
      break;

#if defined(_AVR_E_)||defined(_ARM_)
    case 'V':
      //		        Tbuf_PutChar((uint8_t)value,&RT_COM2);
      break;
#endif
    case 'Z':
      variables[ANS_VARNUM] = value;
      break;
    default:
      BasProg.Err_id = ERR_VARNUM;
  }
}


/*---------------------------------------------------------------------------*/
int32_t
ubasic_get_variable(uint8_t varnum) {
  int32_t r = 0;
  if (varnum >= 0x20 && varnum < (MAX_VARNUM + 0x20)) {
      return variables[(varnum - 0x20)];
  }

  switch (varnum + 'A') {
#ifdef PINA
    case 'A':
      r = PINA;
      break;
#endif  
#ifdef PINB
    case 'B':
      r = PINB;
      break;
#endif
#ifdef PINC
   case 'C':
      r = PINC;
      break;
#endif
#ifdef PIND
    case 'D':
      r = PIND;
      break;
#endif
#ifdef PINE
    case 'E':
      r = PINE;
      break;
#endif
#ifdef PINF
    case 'F':
      r = PINF;
      break;
#endif
#ifdef PING
    case 'G':
      r = PING;
      break;
#endif
#ifdef PINH
    case 'H':
      r = PINH;
      break;
#endif
#ifdef PINI
    case 'I':
      r = PINI;
      break;
#endif
#ifdef PINJ
    case 'J':
      r = PINJ;
      break;
#endif
#ifdef PINK
    case 'K':
      r = PINK;
      break;
#endif
#ifdef PINL
    case 'L':
      r = PINL;
      break;
#endif
    case 'S':
      r = BasProg.Ctrls;
      break;
    case 'T':
      r = BasProg.Flags;
      break;
#if defined(_AVR_E_)||defined(_ARM_)
    case 'U':
      //		      r = ComGetCharW(&RT_COM1);
      break;


    case 'V':
      //		       r = ComGetCharW(&RT_COM2);
      break;
#endif

    case 'Z':
      r = variables[ANS_VARNUM];
      break;
    default:
        BasProg.Err_id = ERR_VARNUM;
  }
  return r;
}

/*----------------------------------------------------*/

uint32_t get_sysinfo(uint8_t id) __attribute__((weak));
uint32_t get_sysinfo(uint8_t id){
  return 0;	
}

/*-----------------function_hack-------------------------*/
int function_hack(uint8_t tk, int32_t *r)  __attribute__((weak));

int function_hack(uint8_t tk, int32_t *r){
  OUTDEV.print(F("not func hack"));
  return false;
} 

int32_t  function_calcul(void) {
  int32_t r = 0;
  uint8_t  FuncToken;
  volatile IO_REG_TYPE *in;
  BasProg.Err_pl = ERR_FUNC;
  FuncToken = tokenizer_func_token();
  tokenizer_next();

  if (function_hack(FuncToken, &r)) return r;  //鍜岀‖浠跺強骞冲彴鏈夊叧鐨勫嚱鏁版墿鍏�

  accept(TOKENIZER_LEFTPAREN);
  switch (FuncToken)
  {
    case FUNC_ABS:                 //abs(r);
      r = expr();
      accept(TOKENIZER_RIGHTPAREN);
      if (r < 0) r *= -1;
      break;
#if defined(EE1LENTH)||defined(EE2LENTH)
    case FUNC_EERD:
      r = expr();
      accept(TOKENIZER_RIGHTPAREN);
      r = eepromRead8((uint16_t)r);
      break;
#endif
	  case FUNC_PULSEIN:                 //pulsein(pin);
      goto func_argc2;
    case FUNC_SHIFTIN:                 //shiftin();
      goto func_argc3;
    case FUNC_MILLIS:                  //millis();
      accept(TOKENIZER_RIGHTPAREN);
      r = millis();
      break;
    case FUNC_MICROS:				  //micros();
      accept(TOKENIZER_RIGHTPAREN);
      r = micros();
      break;
    case FUNC_INFO:
    case FUNC_ANALOGREAD:
    case FUNC_DIGITALREAD:
	case FUNC_INPUT:
      goto func_argc1;
    default:
      BasProg.Err_id = ERR_FUN_NONE;
      OUTDEV.print(F("func not def"));
      break;
  };
  return r;

func_argc1:
  {
    uint16_t p;
    p = expr();
    accept(TOKENIZER_RIGHTPAREN);
    switch (FuncToken) {
      case FUNC_ANALOGREAD:
        r = analogRead(p);
        break;
      case FUNC_DIGITALREAD:
        r = digitalRead(p);
        break;
      case  FUNC_INFO:
        r = get_sysinfo(p);
		break;
	  case FUNC_INPUT:
        r = input_int(p);
    }
    return r;
  }
  
func_argc2:
  {
    uint16_t p, d;
    p = expr();
    if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next(); //skip ','
    d = expr();
    accept(TOKENIZER_RIGHTPAREN);
    switch (FuncToken) {
      case FUNC_PULSEIN:
        r = pulseIn(p, d);
        break;
    }
    return r;
  }

func_argc3:
  {
    uint16_t p, d, v;
    p = expr();
    if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next();
    d = expr();
    if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next();
    v = expr();
    accept(TOKENIZER_RIGHTPAREN);
    switch (FuncToken) {
      case FUNC_SHIFTIN:
        r = shiftIn(p, d, v);
        break;
    }
    return r;
  }
}

/*---------------------------------------------------------------------------*/
static void
jump_linenum(int linenum)
{
  tokenizer_init(BasProg.pBas);
  while (tokenizer_num() != linenum) {
    do {
      do {
        tokenizer_next();
      } while (tokenizer_token() != TOKENIZER_CR &&
               tokenizer_token() != TOKENIZER_ENDOFINPUT);
      if (tokenizer_token() == TOKENIZER_CR) {
         tokenizer_next();
      }
    } while (tokenizer_token() != TOKENIZER_NUMBER);
  }
return;
}

/*----------------------print_statement-----------------------------*/
void print_statement(void)  __attribute__((weak));
void
print_statement(void)
{
  char string[32];
//  lcd.autoscroll();
  int fm = BasProg.print_base;
  do {
    if (tokenizer_token() == TOKENIZER_STRING) {
      tokenizer_string(string, sizeof(string));
#ifdef LCD
    if (GETBIT(BasProg.Ctrls, OUT_LCD)) lcd.print(string);// 
	else OUTDEV.print(string);
#else
    OUTDEV.print(string);
#endif    
	tokenizer_next();
    } else if (tokenizer_token() == TOKENIZER_COMMA) {
#ifdef LCD
      if (GETBIT(BasProg.Ctrls, OUT_LCD)) lcd.print(" ");
	  else OUTDEV.print(" ");
#else
	  OUTDEV.print(" ");
#endif	
      tokenizer_next();
    } else if (tokenizer_token() == TOKENIZER_SEMICOLON) {
      tokenizer_next();
    } else if ((tokenizer_token()   == TOKENIZER_VARIABLE)  ||
               (tokenizer_token()   == TOKENIZER_NUMBER)    ||
               (tokenizer_token()   == TOKENIZER_HEXNUMBER) ||
			   (tokenizer_token()   == TOKENIZER_LEFTPAREN) ||
               (tokenizer_token()   == TOKENIZER_FUNC)) {          //2014.5 add func
#ifdef LCD
      if (GETBIT(BasProg.Ctrls, OUT_LCD)) lcd.print(expr(),fm);
	  else OUTDEV.print(expr(),fm);
#else
      OUTDEV.print(expr(),fm);
#endif	
    } else {
      break;
    }
  } while (tokenizer_token() != TOKENIZER_CR &&
           tokenizer_token() != TOKENIZER_ENDOFINPUT);
#ifdef LCD
  if (!(GETBIT(BasProg.Ctrls, OUT_LCD))) OUTDEV.println();
#else
  OUTDEV.println();
//  lcd.noAutoscroll();
#endif	
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
if_statement(void)
{
  int r;

  accept(TOKENIZER_IF);
  r = relation();
  if (r) {
    if (tokenizer_token() == TOKENIZER_THEN) accept(TOKENIZER_THEN);
    if (tokenizer_token() == TOKENIZER_NUMBER) {
      jump_linenum(tokenizer_num());
    } else {
      statement();
    }
  } else {
    do {
      tokenizer_next();
    } while (tokenizer_token() != TOKENIZER_ELSE &&
             tokenizer_token() != TOKENIZER_CR &&
             tokenizer_token() != TOKENIZER_ENDOFINPUT);
    if (tokenizer_token() == TOKENIZER_ELSE) {
      tokenizer_next();
      if (tokenizer_token() == TOKENIZER_NUMBER) {
        jump_linenum(tokenizer_num());
      } else {
        statement();
      }
    } else if (tokenizer_token() == TOKENIZER_CR) {
      tokenizer_next();
    }
  }
}

/*---------------------------------------------------------------------------*/
static void rem_statement(void) {
  unsigned char token;
  do {
    tokenizer_next();
    token = tokenizer_token();
  } while (token != TOKENIZER_CRLF &&
           token != TOKENIZER_ENDOFINPUT);
  tokenizer_next();
}

/*---------------------------------------------------------------------------*/

int32_t
let_statement(void)
{
  int vi;
  int32_t val;
  vi = tokenizer_variable_num();
  accept(TOKENIZER_VARIABLE);
  int16_t adr = isArray();
  accept(TOKENIZER_EQ);
  val =  expr();
  accept(TOKENIZER_CR);
  if(adr >=0) {
	arrayWrite(vi,adr,val);  
  }else{ 
    ubasic_set_variable(vi,val);
  }
  return val;  
}

/*---------------------------------------------------------------------------*/
static void
gosub_statement(void)
{
  int linenum;
  accept(TOKENIZER_GOSUB);
  linenum = tokenizer_num();
  accept(TOKENIZER_NUMBER);
  accept(TOKENIZER_CR);
  if (gosub_stack_ptr < MAX_GOSUB_STACK_DEPTH) {
    gosub_stack[gosub_stack_ptr] = BasProg.pChar;
    gosub_stack_ptr++;
    jump_linenum(linenum);
  }
}
/*---------------------------------------------------------------------------*/
static void
return_statement(void)
{
  accept(TOKENIZER_RETURN);
  if (gosub_stack_ptr > 0) {
    gosub_stack_ptr--;
    tokenizer_init(gosub_stack[gosub_stack_ptr]);
  } else {
  }
}
/*---------------------------------------------------------------------------*/
static void
next_statement(void)
{
  int var;

  accept(TOKENIZER_NEXT);
  var = tokenizer_variable_num();
  accept(TOKENIZER_VARIABLE);
  if (for_stack_ptr > 0 &&
      var == for_stack[for_stack_ptr - 1].for_variable) {
    ubasic_set_variable(var,
                        ubasic_get_variable(var) + 1);
    if (ubasic_get_variable(var) <= for_stack[for_stack_ptr - 1].to) {
		tokenizer_init(for_stack[for_stack_ptr - 1].pafter);
    } else {
      for_stack_ptr--;
      accept(TOKENIZER_CR);
    }
  } else {
    accept(TOKENIZER_CR);
  }
}
/*---------------------------------------------------------------------------*/
static void
for_statement(void)
{
  int for_variable, to;

  accept(TOKENIZER_FOR);
  for_variable = tokenizer_variable_num();
  accept(TOKENIZER_VARIABLE);
  accept(TOKENIZER_EQ);
  ubasic_set_variable(for_variable, expr());
  accept(TOKENIZER_TO);
  to = expr();
  accept(TOKENIZER_CR);

  if (for_stack_ptr < MAX_FOR_STACK_DEPTH) {
    for_stack[for_stack_ptr].pafter = BasProg.pChar;
    for_stack[for_stack_ptr].for_variable = for_variable;
    for_stack[for_stack_ptr].to = to;
    for_stack_ptr++;
  }
}
/*---------------------------------------------------------------------------*/
static void
end_statement(void)
{
  accept(TOKENIZER_END);
  SETBIT(BasProg.Flags, GLOBLES_END);
}
/*--------------------------------------------------------------------*/
static uint8_t  list_print_prog(uint8_t ch, int16_t adr, uint16_t size) {
  char s[2];
  s[1] = 0;
  if (GETBIT(BasProg.Flags, GLOBLES_NEW_LINE))
  {
    if (isdigit(ch)) {
      SETBIT(BasProg.Flags, IS_LINE_NUM);
    } else {
      CLRBIT(BasProg.Flags, IS_LINE_NUM);
    }
  };

  if ((ch == 0xff) || (ch == '\0')) {
    if (adr == -1) {
      OUTDEV.print(F("ram:"));   OUTDEV.print(size);
      OUTDEV.print(F(" free:")); OUTDEV.print(MAX_PROG_RAM - size);
      OUTDEV.println();
    } else {
      OUTDEV.print(F("eep@"));   OUTDEV.print(adr);
      OUTDEV.print(F(" size:")) ; OUTDEV.print(size);
      OUTDEV.println();
    };
    return 1;   //
  };

  s[0] = ch;
  if (ch == '\n') {
    OUTDEV.print(F("\r"));
    OUTDEV.print(s);
    SETBIT(BasProg.Flags, GLOBLES_NEW_LINE);
  } else {
    if (GETBIT(BasProg.Flags, GLOBLES_NEW_LINE)) //newline
    {
      if (!(GETBIT(BasProg.Flags, IS_LINE_NUM))) //no linenum
      {
        OUTDEV.print(F("   "));
      };
      CLRBIT(BasProg.Flags, GLOBLES_NEW_LINE);
    };
    OUTDEV.print(s);
  };
  return 0;  //
}


/*---list or list eep:/fla:/---------------------------------------------------*/
void list_statement(void) {
  uint32_t i, adr = 0;
  uint8_t ch, file_id = 0, count = 0, t = 1;
  tokenizer_next();
  switch (tokenizer_token())
  {
  case TOKENIZER_EEP:
#ifdef EE_PROG_START_ADR   //have ee2prom save prom
      tokenizer_next();
      if (tokenizer_token() == TOKENIZER_COLON)
      {
        tokenizer_next();
        ch = tokenizer_char();
        if (isdigit(ch)) file_id = ch - '0';
        adr = EE_PROG_START_ADR;
        if (count < file_id) {
          while (t) {
			ch =  eepromRead8(adr++) ;
            if (ch == 0) count++;             		
            if (count == file_id) t = 0;
            if (adr >= (EE_PROG_END_ADR)) return;
          }
        }
        SETBIT(BasProg.Flags, GLOBLES_NEW_LINE);
        for (i = 0; i < MAX_PROG_RAM - 1; i++) {
          if ((adr + i) < EE_PROG_END_ADR) {
            ch = eepromRead8(adr + i);
          } else {
            return;
          }
          if (list_print_prog(ch, adr, i)) return;
        } //end for
      }
#endif
      break;

    default:
      SETBIT(BasProg.Flags, GLOBLES_NEW_LINE);
      for (i = 0; i < MAX_PROG_RAM; i++)
      {
        ch = prog_ram[i];
        if (list_print_prog(ch, -1, i)) return;       //  adr锛濓紞1
      };
      break;
  }
}

/*-----------------------dir------------------------------*/
void dir_statement(void)  __attribute__((weak));
   //have ee2prom save prom
void dir_statement(void) {
#ifdef EE_PROG_START_ADR
  char s[2];
  uint16_t i, adr = 0;
  uint8_t ch, file_id = 0, count = 0, t = 1;
  s[1] = 0;
  tokenizer_next();
  //        ch = tokenizer_char();
  //        if (isdigit(ch)) file_id = ch - '0';
  adr = EE_PROG_START_ADR;
  t = (eepromRead8(adr) == 0xff) ? 0 : 1;
  while (t) {
    OUTDEV.print(count,10);
    OUTDEV.print(F("#->  "));
    count++;
    do {
      s[0] = eepromRead8(adr++);
      if(!(s[0]=='"'))OUTDEV.print(s);
    } while (!(s[0] == 0x0a));
    OUTDEV.println();
    do {
      s[0] = eepromRead8(adr++);
    } while (!(s[0] == 0));
    if (eepromRead8(adr) == 0xff) t = 0;
  }
  OUTDEV.print(count,10);
  OUTDEV.print(F(" files at EEPROM  free: "));
  OUTDEV.println(EE_PROG_END_ADR - adr + 1,10); 
#else
  OUTDEV.println(F("!Not found EEPROM!"));	
#endif
}

/*---------------------------------------------------------------------------*/

//----------------------------attachInterrupt----------------------------------------//
void exterIntServe(uint8_t x)
{
  SETBIT(intes_stack.flags, x);
  if (intes_stack.flags & intes_stack.masks ) SETBIT(events_stack.flags, EVENT_INTE);       // bits set to indicate which event fired
}

#if (EXTERNAL_NUM_INTERRUPTS >0)
void callback_int0(void) {
  exterIntServe(0);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >1)
void callback_int1(void) {
  exterIntServe(1);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >2)
void callback_int2(void) {
  exterIntServe(2);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >3)
void callback_int3(void) {
  exterIntServe(3);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >4)
void callback_int4(void) {
  exterIntServe(4);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >5)
void callback_int5(void) {
  exterIntServe(5);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >6)
void callback_int6(void) {
  exterIntServe(6);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >7)
void callback_int7(void) {
  exterIntServe(7);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >8)
void callback_int8(void) {
  exterIntServe(8);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >9)
void callback_int9(void) {
  exterIntServe(9);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >10)
void callback_int10(void) {
  exterIntServe(10);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >11)
void callback_int11(void) {
  exterIntServe(11);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >12)
void callback_int12(void) {
  exterIntServe(12);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >13)
void callback_int13(void) {
  exterIntServe(13);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >14)
void callback_int14(void) {
  exterIntServe(14);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >15)
void callback_int15(void) {
  exterIntServe(15);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >16)
void callback_int16(void) {
  exterIntServe(16);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >17)
void callback_int17(void) {
  exterIntServe(17);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >18)
void callback_int18(void) {
  exterIntServe(18);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >19)
void callback_int19(void) {
  exterIntServe(19);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >20)
void callback_int20(void) {
  exterIntServe(20);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >21)
void callback_int21(void) {
  exterIntServe(21);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >22)
void callback_int22(void) {
  exterIntServe(22);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >23)
void callback_int23(void) {
  exterIntServe(23);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >24)
void callback_int24(void) {
  exterIntServe(24);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >25)
void callback_int25(void) {
  exterIntServe(25);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >26)
void callback_int26(void) {
  exterIntServe(26);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >27)
void callback_int27(void) {
  exterIntServe(27);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >28)
void callback_int28(void) {
  exterIntServe(28);
}
#endif
#if (EXTERNAL_NUM_INTERRUPTS >29)
void callback_int29(void) {
  exterIntServe(29);
}
#endif

#ifdef __cplusplus
extern "C"{ 
#endif 
void
callback_systic(void){   //for  avr  time0 sysclick callback
  uint8_t i;
  if(timers_stack.masks){             //have one times chnl set
     for (i = 0; i < NUM_TIMER_HANDLERS; i++ )
     {
       if(GETBIT(timers_stack.masks,i))
		   if (!(--timers_stack.clock[i])){
              SETBIT(timers_stack.flags, i);
              SETBIT(events_stack.flags, EVENT_TIMER);        // bits set to indicate which event fired
              timers_stack.clock[i] = timers_stack.interval[i];  //reload timers_stack.clockcounterval
           }
     }
  }	 
}
#ifdef __cplusplus
}//extern "C"{ 
#endif 

typedef void (*interruptCB)(void);
static const interruptCB cbFuncs[] = {
#if (EXTERNAL_NUM_INTERRUPTS >0)
  callback_int0,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >1)
  callback_int1,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >2)
  callback_int2,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >3)
  callback_int3,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >4)
  callback_int4,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >5)
  callback_int5,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >6)
  callback_int6,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >7)
  callback_int7,
#endif 
#if (EXTERNAL_NUM_INTERRUPTS >8)
  callback_int8,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >9)
  callback_int9,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >10)
  callback_int10,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >11)
  callback_int11,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >12)
  callback_int12,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >13)
  callback_int13,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >14)
  callback_int14,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >15)
  callback_int15,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >16)
  callback_int16,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >17)
  callback_int17,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >18)
  callback_int18,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >19)
  callback_int19,
#endif
#if (EXTERNAL_NUM_INTERRUPTS >20)
  callback_int20,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >21)
  callback_int21,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >22)
  callback_int22,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >23)
  callback_int23,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >24)
  callback_int24,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >25)
  callback_int25,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >26)
  callback_int26,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >27)
  callback_int27,
#endif 
#if (EXTERNAL_NUM_INTERRUPTS >28)
  callback_int28,
#endif  
#if (EXTERNAL_NUM_INTERRUPTS >29)
  callback_int29,
#endif
};


void
events_statement( void ){ 
  uint16_t  activeevents, i;
  uint16_t eventlinenum = 0;

  activeevents = events_stack.flags & events_stack.masks;
  SETBIT(BasProg.Flags, IN_EVENTS_PROC);   //set in events flag
  if (GETBIT(activeevents, EVENT_TIMER))             //is TIMER event?
  {
    for ( i = 0; i < NUM_TIMER_HANDLERS; i++ )    //timer 0~7 every counter events
    {
      if (timers_stack.flags & BIT(i) & timers_stack.masks) //get first event
      {
        eventlinenum = timers_stack.gosub_line[i];       //get it process linenum
        CLRBIT(timers_stack.flags, i);                   //clear it flag
        break;                                           //exit for
      }
    }
    if (!(timers_stack.flags & timers_stack.masks))        //clear timer flag bit in events while processed all timer events
       CLRBIT(events_stack.flags, EVENT_TIMER);
  }


  if (!eventlinenum && (GETBIT(activeevents, EVENT_INTE)))  //extint
  {
    for ( i = 0; i < NUM_INTE_HANDLERS; i++ )    //exti 0~7 every counter events
    {
      if (intes_stack.flags & BIT(i) & intes_stack.masks) //get first event
      {
        eventlinenum = intes_stack.gosub_line[i];       //get it process linenum
        CLRBIT(intes_stack.flags, i);                   //clear it flag
        break;                                           //exit for
      }
    }
    if (!(intes_stack.flags & intes_stack.masks))        //clear inte flag bit in events while processed all timer events
      CLRBIT(events_stack.flags, EVENT_INTE);        
  }

  if ( eventlinenum )    // have one events
  {
    SETBIT(BasProg.Flags, IN_EVENTS_PROC);
    events_stack.save_ptr = BasProg.pChar;          //save return pChar
//	events_stack.save_token = tokenizer_token();
    jump_linenum(eventlinenum);                     //gosub linenum
  }

}


static void iret_statement(void) {
  tokenizer_next();  //  accept TOKENIZER_CRLF
  if (!(events_stack.save_ptr == NULL)) //[MAX_EVENTS_STACK]
  { 
    CLRBIT(BasProg.Flags, IN_EVENTS_PROC);
//    BasProg.pChar        =  events_stack.save_ptr;   //restore pChar
//	BasProg.CurrentToken = events_stack.save_token;
    tokenizer_init((const char *)events_stack.save_ptr);
//	events_stack.save_token =  TOKENIZER_END;
    events_stack.save_ptr = NULL;
  }
  else
  {
    BasProg.Err_id = ERR_IRET_ADR;
  };
}

/*--------exp:  20 timer(x) on/off----------------------------------------*/
//#define  TIMSK1 TIMSK

static void
timer_statement(void) { //  time counter events
  uint8_t token;
  tokenizer_next();
 
  timers_stack.timernum = expr(); //timer events no

  if (timers_stack.timernum > (NUM_TIMER_HANDLERS - 1))
  {
    BasProg.Err_id = ERR_TIMER_NUM;
    return;
  };

  CLRBIT(events_stack.flags, EVENT_TIMER); // clear flag in eventflags to clear any past timer events of this type
  CLRBIT(timers_stack.flags, timers_stack.timernum);

  token = tokenizer_token();
  tokenizer_next();
  accept(TOKENIZER_CRLF);

     switch(token)
      {
   	  case TOKENIZER_ON:
   				SETBIT(timers_stack.masks, timers_stack.timernum);
   				SETBIT(events_stack.masks,EVENT_TIMER);           //  enable timer event
   				timers_stack.clock[timers_stack.timernum] = timers_stack.interval[timers_stack.timernum]; //脳掳???????梅                                                              // setup timer1
//   				TIM_Cmd(TIM2, ENABLE);
      		   break;

   	   case TOKENIZER_OFF:
   				CLRBIT(timers_stack.masks,timers_stack.timernum);
   				if(!timers_stack.masks)                      //
   					CLRBIT(events_stack.masks,EVENT_TIMER);   // clear flag in eventmasks to disable event
   		   break;

   	   default:
   	       BasProg.Err_id = ERR_TIMER;
   	      return;
   	}
 
}

/*--------exp:  20 inte(x) on/off----------------------------------------*/
static void inte_statement(void) { // exter interrupt events
  uint8_t token,inte_num;
  tokenizer_next();  //skip inte
  inte_num= expr();   //chnl
  token = tokenizer_token();
  tokenizer_next();
   if(inte_num > (NUM_INTE_HANDLERS-1))
      {
        BasProg.Err_id = ERR_INTE_NUM;
    	return;
      };

  CLRBIT(events_stack.flags, EVENT_INTE); // clear flag in eventflags to clear any past timer events of this type
  CLRBIT(intes_stack.flags, inte_num);
  accept(TOKENIZER_CRLF);
  switch (token)
  {
    case TOKENIZER_ON:
       SETBIT(events_stack.masks,EVENT_INTE);       //  enable exti event
       SETBIT(intes_stack.masks, inte_num);
/*       EXTI_InitStruct.EXTI_Trigger = intes_stack.val[inte_num];
      			EXTI_Init(&EXTI_InitStruct);
      			EXTI_Cmd(&EXTI_InitStruct, ENABLE);
*/      break;
    case TOKENIZER_OFF:
       CLRBIT(intes_stack.masks,inte_num);
      if(!intes_stack.masks)                      //猫?2?1?卤??
        CLRBIT(events_stack.masks,EVENT_INTE);   // clear flag in eventmasks to disable event
      //  EXTI_Cmd(&EXTI_InitStruct, DISABLE);
      break;

    default:
      BasProg.Err_id = ERR_INTE;
      return;
  }


}

/*----exp: 10 on time(0)/inte(6) = xxxx  gosub 1000------------------------*/
static void
on_statement(void) {       //exvents set
  uint8_t token, chnl;
  uint16_t v, v1, linenum;
  BasProg.Err_pl = ERR_ON;
  tokenizer_next();
  //  tokenizer_next();
  token = tokenizer_token();   //timer or inte
  tokenizer_next();
  chnl = expr();
  accept(TOKENIZER_EQ);
  v = expr();
   
  accept(TOKENIZER_GOSUB);
  linenum = expr();
  accept(TOKENIZER_CRLF);

  switch (token)
  {
    case TOKENIZER_TIMER:
      if (chnl > (NUM_TIMER_HANDLERS - 1))
      {
        BasProg.Err_id = ERR_TIMER_NUM;
        return;
      };
      timers_stack.interval[chnl] = v;          // time counter reload val ms;
      timers_stack.gosub_line[chnl] = linenum;  // callbak linenum for TIMER()
      break;
    case TOKENIZER_INTE:
      if (!(chnl > (NUM_INTE_HANDLERS - 1))){
//      if (chnl < EXTERNAL_NUM_INTERRUPTS){
 //      intes_stack.val[chnl] = (v & 0x03);          // exti mode  L 01 F 10 HL 11 LH;
//#define CHANGE  1  FALLING 2  RISING  3
      intes_stack.gosub_line[chnl] = linenum;       // callbak linenum for INTE()
      attachInterrupt(chnl, cbFuncs[chnl],(int)(v & 0x03));    //attachInterrupt(chnl,ISRi,mode)
	  }else{
        BasProg.Err_id = ERR_INTE_NUM;
        return;
      };
      break;
      /*  case TOKENIZER_PIN:         // exp:   ON PIN(adr,bits,0x80,ctrl
      	  case TOKENIZER_COM:
      	  case TOKENIZER_IIC:
      	  case TOKENIZER_SPI:
        break;
      */
  }
}


#ifdef EE_PROG_START_ADR   //have ee2prom save prom
uint8_t load_eep_prog(int8_t file_id)  __attribute__((weak));
uint8_t load_eep_prog(int8_t file_id) {
  uint16_t adr, i;
  uint8_t ch, count = 0, t = 1;
  adr = EE_PROG_START_ADR;
  if (count < file_id) {
    while (t) {
      if (eepromRead8(adr++) == 0) count++;
      if (count == file_id) t = 0;
      if (adr > EE_PROG_END_ADR) goto ERR;
    }
  }
  for (i = 0; i < MAX_PROG_RAM - 1; i++) {
    if ((adr + i) <= EE_PROG_END_ADR) {
      ch = eepromRead8(adr + i);
      if (ch == 0xff) ch = '\0';
      prog_ram[i] = ch;
      if (ch == '\0')
      {
	    BasProg.pRam =(char *) &prog_ram[i++]+1;
        BasProg.lenRam = sizeof(prog_ram)-i;		  
        return ERR_NONE;
      }
    } else {
      goto ERR;
    }
  }
ERR:
  return ERR_LOAD_DEV;
}
#endif

void load_com_prog() {
  uint16_t i;
  uint8_t ch;
  int8_t end = 0;
  ch = 0xff;
  i = 0;
  while ((!(ch  == '\0')) && (i < MAX_PROG_RAM - 1))
  {
    ch = inchar();
    if ((ch == 0xff) || (ch == KEYDEF_ESC) || (ch == KEYDEF_CTRL_C)) ch = '\0'; //KEYDEF_CTRL_C end

    switch (end)
    {
      case 0:        //??铆篓脳?路?
        if (ch == KEYDEF_CR)
        {
          end = 1;  // 锚脳鈥�?cr
        }
        else if (ch == KEYDEF_LF)
        {
          end = -2;  // 锚脳鈥�?lf
        };
        break;
      case 1:    // last cr
        if (ch == KEYDEF_CR) // crcr
        {
          end = 5;  // ?谩锚?
        }
        else if (ch == KEYDEF_LF) //
        {
          end = -3;    // 锚脳鈥�? crlf
        }
        else
        {
          end = 0;  //cr+x
          prog_ram[i++] = '\n'; //skip  \r
        };
        break;
      case -2:  // lf
        if (ch == KEYDEF_LF) //lf+lf
        {
          end = 5;  // ?谩锚?
        }
        else
        {
          end = 0;  //??end
        };
        break;
      case -3:
        if (ch == KEYDEF_CR)  //cr lf cr
        {
          end = 4;
        }
        else
        {
          end = 0;  // cr lf x
        };
        break;
      case 4:
        if (ch == KEYDEF_LF) //cr lf cr lf
        {
          end = 5;  // ?谩锚?
        }
        break;
    };
    if (end == 5) {
      prog_ram[i++] = '\n'; // add  \n
      ch = '\0';
      end = 0 ;
    };
    if (end < 1)	 prog_ram[i++] = ch;
    if (ch == '\0'){
	  BasProg.pRam =(char *) &prog_ram[i]+1;
      BasProg.lenRam = sizeof(prog_ram)-i;
	break;
	}
  }
}


void save_statement(void)  __attribute__((weak));
/*-----save eep:--------------------------------------------*/
void save_statement(void) {
#ifdef EE_PROG_START_ADR   //have ee2prom save prom
  uint16_t i, adr = 0;
  uint8_t ch, t = 1;
  tokenizer_next();
  switch (tokenizer_token())
  {
    case TOKENIZER_EEP:
      tokenizer_next();
      if (tokenizer_token() == TOKENIZER_COLON)
      {
        tokenizer_next();
        //        ch = tokenizer_char();
        adr = EE_PROG_START_ADR;
        while (t) {
          if (eepromRead8(adr) == 0xff) {
            t = 0;
          } else {
            adr++;
          }
          if (adr >=(EE_PROG_END_ADR)) goto ERR;
        }

        for (i = 0; i < MAX_PROG_RAM - 1; i++)
        {
          ch = prog_ram[i];
          if (adr+i >= (EE_PROG_END_ADR)) goto ERR;
          if  (ch == '\0') {
            eepromWrite8(adr + i++, ch);
            ch = 0xff;
          }
          if ((adr + i) <=EE_PROG_END_ADR) {
            eepromWrite8(i + adr, ch);
          } else {
            goto ERR;
          }
          if  (ch == 0xff) return;
        }
      };
      break;
  }
ERR:
  BasProg.Err_id = ERR_SAVE_DEV;
#else
  OUTDEV.println(F("!Not found EEPROM!"));	
#endif
}


void load_statement(void)  __attribute__((weak));
/*-----load or load eep:/fla:/com:--------------------------------------------*/
void load_statement(void) {
  uint16_t adr = 0;
  // uint16_t i;
  uint8_t ch;
  tokenizer_next();
  switch (tokenizer_token())
  {
    case TOKENIZER_EEP:
#ifdef EE_PROG_START_ADR
      tokenizer_next();
      if (tokenizer_token() == TOKENIZER_COLON)
      {
        tokenizer_next();
        ch = tokenizer_char();
        if (isdigit(ch)) adr = ch - '0';
        load_eep_prog(adr);
      } else {
        BasProg.Err_id = ERR_NO_COLON;
      };
#endif	  
      break;
    case TOKENIZER_COM:
      tokenizer_next();
      if (tokenizer_token() == TOKENIZER_COLON)
      {
        load_com_prog();
      }
      else
      {
        BasProg.Err_id = ERR_NO_COLON;
      };
      break;
    default:
      BasProg.Err_id = ERR_LOAD_DEV;
      break;
  }
}


/*----------------------------------------------------*/
// user can be rebuild this func
int
statement_hack(int) __attribute__((weak));
int
statement_hack(int tk) {
  OUTDEV.print(F("not hack"));
  return false;
}
/*----------------------------------------------------*/
void goto_statement(void){
   tokenizer_next();
   uint16_t num = expr();
//   accept(TOKENIZER_CRLF);   
   jump_linenum(num);
}

void
statement(void) {
  int token;

  token = tokenizer_token();
  if (statement_hack(token)) return;	 //鍜岀‖浠跺拰骞冲彴鏈夊叧鐨勫懡浠ゆ墿鍏呮帴鍙�
  switch (token) {
    case TOKENIZER_DIGITALTOGGLE:
    case TOKENIZER_NOTONE:
    case TOKENIZER_DELAYMS:
    case TOKENIZER_DELAYUS:
       goto statement_argc1;
    case TOKENIZER_DIGITALWRITE:
    case TOKENIZER_PINMODE:
    case TOKENIZER_ANALOGWRITE:
    case TOKENIZER_TONE:
#ifdef LCD
	case TOKENIZER_CSR:
#endif
    case TOKENIZER_EEWR:
      goto statement_argc2;
#ifdef LCD
    case TOKENIZER_LCD:
      accept(TOKENIZER_LCD);
      SETBIT(BasProg.Ctrls, OUT_LCD);
	  print_statement();
	  break;
#endif
    case TOKENIZER_PRINT:
      accept(TOKENIZER_PRINT);
#ifdef LCD
	  CLRBIT(BasProg.Ctrls, OUT_LCD);
#endif	  
    /* Fall through. */
    case TOKENIZER_STRING:     //call print_statement printed.
      print_statement();
      break;
    case TOKENIZER_REM:
      rem_statement();
      break;
    case TOKENIZER_ON:
      on_statement();
      break;
    case TOKENIZER_TIMER:
      timer_statement();
      break;
    case TOKENIZER_INTE:
      inte_statement();
      break;
    case TOKENIZER_IRET:
      iret_statement();
      break;
    case TOKENIZER_IF:
      if_statement();
      break;
    case TOKENIZER_GOTO:
	  goto_statement();
      break;
    case TOKENIZER_GOSUB:
      gosub_statement();
      break;
    case TOKENIZER_RETURN:
      return_statement();
      break;
    case TOKENIZER_FOR:
      for_statement();
      break;
    case TOKENIZER_NEXT:
      next_statement();
      break;
#ifdef LCD
    case TOKENIZER_CLS:
	  lcd.clear();
	  break;
#endif
	case TOKENIZER_END:
      end_statement();
      break;
    case TOKENIZER_LET:
      accept(TOKENIZER_LET);
    /* Fall through. */
    case TOKENIZER_VARIABLE:
      let_statement();
      break;
    default:
      break;
  }
  return;

statement_argc1:
  {
    int16_t p;
    tokenizer_next();
    p = expr();
    accept(TOKENIZER_CR);
    switch (token) {
      case TOKENIZER_DIGITALTOGGLE:
        digitalToggle(p);
        break;
      case TOKENIZER_NOTONE:
        noTone(p);
        break;
      case TOKENIZER_DELAYMS:
        delay(p);
        break;
      case TOKENIZER_DELAYUS:
        delayMicroseconds(p);
        break;
    }
    return;
  }

statement_argc2:
  {
    uint16_t p;
    uint16_t val;
    tokenizer_next();
    p = expr();
    if (tokenizer_token() == TOKENIZER_COMMA) tokenizer_next(); //skip ','
    val = (uint16_t) expr();
    accept(TOKENIZER_CR);
    switch (token) {
      case TOKENIZER_PINMODE:
        pinMode(p, val);
        break;
      case TOKENIZER_ANALOGWRITE:
        analogWrite(p, val);
        break;
#ifdef LCD
	  case TOKENIZER_CSR:
	  	lcd.setCursor((uint8_t)p,(uint8_t)val);
	  break;
#endif
      case TOKENIZER_DIGITALWRITE:
        digitalWrite(p, val);
        break;
      case TOKENIZER_TONE:
        tone(p, val);
        break;
#if defined(EE1LENTH)||defined(EE2LENTH)
      case TOKENIZER_EEWR:
        eepromWrite8(p, val);
        break;
#endif
		}
    return;
  }
}

/*---------------------------------------------------------------------------*/
void events(void); 
void
ubasic_run(void)
{
  ubasic_init((const char *)prog_ram);
  do {
    if (breakcheck())    //break check
    {
      OUTDEV.println(F("Break"));
      return;
    };
    if (BasProg.Err_id)                               //err check
    {
      OUTDEV.println(F("Err_Bas:"));
      OUTDEV.println(BasProg.Err_id);
      BasProg.Err_id = 0;
      return;
    }
	//     line_statement();
    if (!(GETBIT(BasProg.Flags, IN_EVENTS_PROC)) &&   //not in events proc
      (events_stack.flags & events_stack.masks))  events_statement();     //happen  event
      // process events
    if (tokenizer_token() == TOKENIZER_NUMBER) tokenizer_next(); //line num can null
    statement();
  } while (!ubasic_finished());
  return;
}
/*---------------------------------------------------------------------------*/
void ubasic_setup(void){
  uint8_t eep_id;	
#ifdef LCD
  lcd.clear();
  lcd.begin(20, 4);
  lcd.print(F("  DuinoBasic V"));
  lcd.print(ARDUINO_BASIC_VAR);
  lcd.setCursor(0,1);
#endif
  BasProg.pRam =(char *)  &prog_ram[sizeof(PROG_RAM_DEFAULT)-1] +1;
  BasProg.lenRam = sizeof(prog_ram)-sizeof(PROG_RAM_DEFAULT);
  BasProg.print_base = 10;  //缺省 10 进制print
#if defined(EE1LENTH)||defined(EE2LENTH)
  switch (eepromRead8(AUTO_RUNADR)){
	  case 2:
	    eep_id = eepromRead8(AUTO_RUNADR+1);
        load_eep_prog(eep_id);	
	  case 1:	
	    ubasic_run();   //autorun: ram 127,
		break;
  };
#endif  
}
