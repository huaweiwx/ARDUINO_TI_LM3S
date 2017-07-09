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

#if ARDUINO
	#include "arduino.h"
#else
	#include <stdint.h>
//	#include <stdbool.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>	
#endif

#include "mytypedef.h"
#include "tokenizer.h"
#include "mybasic.h"


#ifndef F
#define F(x)  (x)
#endif
#include "WString.h"

extern sGLOBEL_T BasProg;

struct keyword_token {
  const char*  keyword;
  unsigned char token;
};

#if defined(__arm__)

static const struct keyword_token  keywords[] =
{
  {"ar",	FUNC_ANALOGREAD},			//analogRead()
  {"aw",	TOKENIZER_ANALOGWRITE},   //analogWrite
  {"abs",	FUNC_ABS}, 			      //abs();
  {"pm",	TOKENIZER_PINMODE},       //pinMode
  {"puls",  FUNC_PULSEIN},			  //pulsein(pin);
  {"dr",    FUNC_DIGITALREAD},			//digitalRead()
  {"dw",    TOKENIZER_DIGITALWRITE},	//digitalWrite
  {"dt",	TOKENIZER_DIGITALTOGGLE},	//digitalToggle
  {"mlis",  FUNC_MILLIS}, 			 //millis();
  {"mcrs",  FUNC_MICROS},				//micros();
  {"shfti", FUNC_SHIFTIN}, 			//shiftin();
  {"shfto", TOKENIZER_SHIFTOUT},   //shiftout();
  {"tone",  TOKENIZER_TONE},            //tone(pin)
  {"ntone", TOKENIZER_NOTONE},        //notone()
  {"input", FUNC_INPUT},     // input();

  {"for",	TOKENIZER_FOR},
  {"to",	TOKENIZER_TO},
  {"next",	TOKENIZER_NEXT},
  {"if"	, 	TOKENIZER_IF},
  {"then",	TOKENIZER_THEN},
  {"else",	TOKENIZER_ELSE},
  {"goto",	TOKENIZER_GOTO},

  {"gosub",	TOKENIZER_GOSUB},
  {"return",	TOKENIZER_RETURN},

  {"dms",	  TOKENIZER_DELAYMS},
  {"dus",	  TOKENIZER_DELAYUS},
  {"timer",	  TOKENIZER_TIMER},
  {"inte",	  TOKENIZER_INTE},
  {"on",	  TOKENIZER_ON},
  {"off",	  TOKENIZER_OFF},
  {"iret",	  TOKENIZER_IRET},

  {"print",	TOKENIZER_PRINT},
  {"?",		TOKENIZER_PRINT},
  {"info",	FUNC_INFO},
  {"sys",	TOKENIZER_SYS},
  {"set",	TOKENIZER_SET},
/*  {"peek",  FUNC_PEEK},*/
#ifdef LCD                    //for lcd 2004 1602
  {"lcd",  TOKENIZER_LCD},
  {"csr",  TOKENIZER_CSR},
  {"cls",  TOKENIZER_CLS},
#endif  
  {"eer",	FUNC_EERD},
  {"eew",	TOKENIZER_EEWR},

/*  {"clear",	TOKENIZER_CLEAR},*/
  {"new",		TOKENIZER_NEW},
  {"eep",		TOKENIZER_EEP},
  {"com",		TOKENIZER_COM},
  {"run",		TOKENIZER_RUN},
  {"list",		TOKENIZER_LIST},
  {"end",		TOKENIZER_END},
  {"let",  		TOKENIZER_LET},
  {"load",		TOKENIZER_LOAD},
/*  {"help",	TOKENIZER_HELP},*/
/*  {"reset", TOKENIZER_RESET} ,*/
  {"save",		TOKENIZER_SAVE},
  {"dir", 		TOKENIZER_DIR},
  { NULL,		TOKENIZER_ERROR}
};
#pragma message("arduino basic for arm")
#elif defined(__AVR__)  //for AVR Seriers
const uint8_t PROGMEM keywords_PGM[] =
{
  FUNC_ANALOGREAD,			'a','r',0,			//analogRead()
  TOKENIZER_ANALOGWRITE,	'a','w',0,			//analogWrite
  FUNC_ABS,					'a','b','s',0,		//abs();
  TOKENIZER_PINMODE,		'p','m',0,	       	//pinMode
  FUNC_PULSEIN,				'p','u','l','s',0, 	//pulsein(pin);
  FUNC_DIGITALREAD,			'd','r',0,			//digitalRead()
  TOKENIZER_DIGITALWRITE,	'd','w',0,			//digitalWrite
  TOKENIZER_DIGITALTOGGLE,	'd','t',0,			//digitalToggle
  FUNC_MILLIS,				'm','l','i','s',0,   //millis();
  FUNC_MICROS,				'm','c','r','s',0,	//micros();
  FUNC_SHIFTIN,				's','h','f','t','i',0,	//shiftin();
  TOKENIZER_SHIFTOUT,		's','h','f','t','o',0,   //shiftout();
  TOKENIZER_TONE,			't','o','n','e',0,       //tone(pin)
  TOKENIZER_NOTONE,			'n','t','o','n','e',0,	//notone()
  FUNC_INPUT,				'i','n','p','u','t',0,     // input();

  TOKENIZER_FOR,			'f','o','r',0,           //for
  TOKENIZER_TO,				't','o',0,
  TOKENIZER_NEXT,			'n','e','x','t',0,  
  TOKENIZER_IF,				'i','f',0,
  TOKENIZER_THEN,			't','h','e','n',0,
  TOKENIZER_ELSE,			'e','l','s','e',0,
  TOKENIZER_GOTO,			'g','o','t','o',0,

  TOKENIZER_GOSUB,			'g','o','s','u','b',0,

  TOKENIZER_DELAYMS,		'd','m','s',0,
  TOKENIZER_DELAYUS,		'd','u','s',0,
  TOKENIZER_TIMER,			't','i','m','e','r',0,
  TOKENIZER_INTE,			'i','n','t','e',0,
  TOKENIZER_ON,				'o','n',0,
  TOKENIZER_OFF,			'o','f','f',0,
  TOKENIZER_IRET,			'i','r','e','t',0,

  TOKENIZER_PRINT,			'p','r','i','n','t',0,
//  TOKENIZER_PRINT,          '?',0,
  FUNC_INFO,				'i','n','f','o',0,
  TOKENIZER_SYS,			's','y','s',0,
  TOKENIZER_SET,			's','e','t',0,       //set
/*  FUNC_PEEK,	'p','e','e','k',0,         */
#ifdef LCD                    //for lcd 2004 1602
  TOKENIZER_LCD,			'l','c','d',0,      //lcd print
  TOKENIZER_CSR,			'c','s','r',0,      //csr
  TOKENIZER_CLS,			'c','l','s',0,
#endif  
  FUNC_EERD,				'e','e','r',0,
  TOKENIZER_EEWR,			'e','e','w',0,

/*  {"clear",	TOKENIZER_CLEAR},*/
  TOKENIZER_NEW,			'n','e','w',0,
  TOKENIZER_EEP,			'e','e','p',0,
  TOKENIZER_COM,			'c','o','m',0,
  TOKENIZER_RUN,			'r','u','n',0,
  TOKENIZER_LIST,			'l','i','s','t',0,
  TOKENIZER_END,			'e','n','d',0,
  TOKENIZER_LET,			'l','e','t',0,
  TOKENIZER_LOAD,			'l','o','a','d',0,
/*  {"help",	TOKENIZER_HELP},*/
  TOKENIZER_SAVE,			's','a','v','e',0,
  TOKENIZER_DIR,			'd','i','r',0,
/*  {"reset", TOKENIZER_RESET},*/
  0,0,0
};

#define READ_PGP(p)    (pgm_read_byte(keywords_PGM+(p)))
 #pragma message("basic for avr mcu")	

#else
   #error "not support this mcu"
#endif
/* void arduino_prints(char * s);
  void arduino_printi(int32_t i); */

//---------------------------------------------------------------------
// Programs
//---------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
static uint8_t
singlechar(void) {
  if ((uint8_t)*BasProg.pChar > 0x7f) {                  //token
    return (uint8_t)((*BasProg.pChar) & 0x7f);  //basic token |= 0x80
  } else  if (*BasProg.pChar == '=') {
    return TOKENIZER_EQ;
  } else if (*BasProg.pChar == '+') {
    return TOKENIZER_PLUS;
  } else if (*BasProg.pChar == '-') {
    return TOKENIZER_MINUS;
  } else if (*BasProg.pChar == '*') {
    return TOKENIZER_ASTR;
  } else if (*BasProg.pChar == '/') {
    return TOKENIZER_SLASH;
  } else if (*BasProg.pChar == '(') {
    return TOKENIZER_LEFTPAREN;
  } else if (*BasProg.pChar == ')') {
    return TOKENIZER_RIGHTPAREN;
  } else if (*BasProg.pChar == '[') {
    return TOKENIZER_LEFTSPAREN;
  } else if (*BasProg.pChar == ']') {
    return TOKENIZER_RIGHTSPAREN;
  } else if (*BasProg.pChar == '&') {
    return TOKENIZER_AND;
  } else if (*BasProg.pChar == '|') {
    return TOKENIZER_OR;
  } else if (*BasProg.pChar == '%') {
    return TOKENIZER_MOD;
  } else if (*BasProg.pChar == '<') {
    return TOKENIZER_LT;
  } else if (*BasProg.pChar == '>') {
    return TOKENIZER_GT;
  } else if (*BasProg.pChar == ';') {
    return TOKENIZER_SEMICOLON;
  } else if (*BasProg.pChar == ',') {
    return TOKENIZER_COMMA;
  } else if (*BasProg.pChar == '@') {
    return TOKENIZER_AT;
  } else if (*BasProg.pChar == ':') {
    return TOKENIZER_COLON;
  } else if (*BasProg.pChar == '?') {
    return TOKENIZER_PRINT;
  } else if (*BasProg.pChar  == '\n') { //0x0a
    return TOKENIZER_CRLF;
  } else if (*BasProg.pChar  == '\r') { //0x0d
    return TOKENIZER_CRLF;
  } else if (*BasProg.pChar  == '#') {
    return TOKENIZER_CLOSE;
  } else if (*BasProg.pChar  == '!') {
    return TOKENIZER_IO;
  } else if (*BasProg.pChar  == '^') {
    return TOKENIZER_POWER;
  }
  return 0;
}


/*---------------------------------------------------------------------------*/
static uint8_t
get_next_token(void)
{
  struct keyword_token const *kt;
  int i;
  if (*BasProg.pChar == 0) {
    return TOKENIZER_ENDOFINPUT;
  }
  if ((*BasProg.pChar == '0') &&       //hex_num
      ((BasProg.pChar[1] == 'x') ||
       (BasProg.pChar[1] == 'X'))) {
    for (i = 2; i < MAX_HEXNUMLEN; ++i)
    {
      if (!isxdigit(BasProg.pChar[i]))
      {
        if (i > 2)
        {
          BasProg.pNext = BasProg.pChar + i;
          return TOKENIZER_HEXNUMBER;
        } else {
          return TOKENIZER_ERROR;  //
        }
      }
      if (!isxdigit(BasProg.pChar[i])) //
      {
        return TOKENIZER_ERROR;
      }
    }
    return TOKENIZER_ERROR;
  } else   if (isdigit(*BasProg.pChar)) {
    for (i = 0; i < MAX_NUMLEN; ++i) {
      if (!isdigit(BasProg.pChar[i])) {
        if (i > 0) {
          BasProg.pNext = BasProg.pChar + i;
          return TOKENIZER_NUMBER;
        } else {

          return TOKENIZER_ERROR;
        }
      }
      if (!isdigit(BasProg.pChar[i])) {

        return TOKENIZER_ERROR;
      }
    }
    //    printf("get_next_token: error due to too long number\n");
    return TOKENIZER_ERROR;
  } else if (singlechar()) {
    BasProg.pNext = BasProg.pChar + 1;
    return singlechar();
  } else if (*BasProg.pChar == '"') {
    BasProg.pNext = BasProg.pChar;
    do {
      ++BasProg.pNext;
    } while (*BasProg.pNext != '"');
    ++BasProg.pNext;
    return TOKENIZER_STRING;
  } else {
#ifndef _AVR_IO_H_
    for (kt = keywords; kt->keyword != NULL; ++kt) {
      if (strncmp(BasProg.pChar, kt->keyword, strlen(kt->keyword)) == 0) {
        BasProg.pNext = BasProg.pChar + strlen(kt->keyword);
        if (kt->token > FUNC_START)
        {
          BasProg.CurrentFuncToken = kt->token;
          return TOKENIZER_FUNC;
        } else {
          return kt->token;
        }
      }
    }
#else  //for  avr
   int j=1;  //比较字符从第一个开始，第0为 token
   uint8_t token;
   token = READ_PGP(0);   //read frist 
   while(token) {                    //until token == 0,notfound!;
     i=0;
	 while(BasProg.pChar[i++] == READ_PGP(j++)); //对每个token 逐个比较字符,直到遇\0
     if (READ_PGP(j-1)) {  //上个非0，不同；
  	   while(READ_PGP(j++));   //跳过，直到0
       token= READ_PGP(j++);    //读并比较下一个非0 token； 		
	 }else{    ////上个为0，找到；
	   BasProg.pNext = BasProg.pChar+i-1;
       if (token > FUNC_START)
       {
          BasProg.CurrentFuncToken = token;
          return TOKENIZER_FUNC;
       } else {
           return token;
       }
     } 
   }//not found & next   	
#endif	
  }

  if ((*BasProg.pChar >= 'a' && *BasProg.pChar <= 'z') ||
      (*BasProg.pChar >= 'A' && *BasProg.pChar <= 'Z')) {
    BasProg.pNext = BasProg.pChar + 1;
    return TOKENIZER_VARIABLE;
  }

  return TOKENIZER_ERROR;
}


/*---------------------------------------------------------------------------*/
void
tokenizer_init(const char *program)
{
  BasProg.pChar = (char *) program;
  while (*BasProg.pChar == ' ') {
    ++BasProg.pChar;
  }  //skip space
  BasProg.CurrentToken = get_next_token();
  //  BasProg.CurrentToken = tokenizer_next();
}
/*---------------------------------------------------------------------------*/
uint8_t
tokenizer_token(void) {
  return BasProg.CurrentToken;
}
uint8_t tokenizer_func_token(void) {
  return BasProg.CurrentFuncToken;
}

/*---------------------------------------------------------------------------*/
void
tokenizer_next(void) {

  if (tokenizer_finished()) {
    return;
  }
  //  printf("tokenizer_next: %p\n", BasProg.pNext);
  BasProg.pChar = BasProg.pNext;
  while (*BasProg.pChar == ' ') {
    ++BasProg.pChar;
  }
  BasProg.CurrentToken = get_next_token();
  //  printf("tokenizer_next: '%s' %d\n", ptr, BasProg.CurrentToken);
  return;
}
/*---------------------------------------------------------------------------*/
int32_t
tokenizer_num(void) {
  return atol(BasProg.pChar);
}

/*----------------------------------------------------------------------------*/
uint8_t todigit(char x) {
  if (x >= '0' && x <= '9') return x - '0';
  if (x >= 'a' && x <= 'f') return x - 'a' + 10;
  if (x >= 'A' && x <= 'F') return x - 'A' + 10;
  return 0;
}

/*****************************************************************************/
//hex2i
int32_t tokenizer_hexnum(void) {
  int32_t xval = 0;
  uint8_t i, val;

  for (i = 2; i < MAX_HEXNUMLEN; i++) //i=2 skip  0x or 0X
  {
    if (isxdigit(BasProg.pChar[i])) {
      val = todigit(BasProg.pChar[i]);
      xval = xval * 16 + val;
    } else {
      break;
    }
  }
  return xval;
}// hex2i

uint8_t tokenizer_char(void) {
  return *BasProg.pChar++;
}

/*---------------------------------------------------------------------------*/
void
tokenizer_string(char *dest, int len)
{
  char *string_end;
  int string_len;

  if (tokenizer_token() != TOKENIZER_STRING) {
    return;
  }
  string_end = strchr(BasProg.pChar + 1, '"');
  if (string_end == NULL) {
    return;
  }
  string_len = string_end - BasProg.pChar - 1;
  if (len < string_len) {
    string_len = len;
  }
  memcpy(dest, BasProg.pChar + 1, string_len);
  dest[string_len] = 0;
}
/*---------------------------------------------------------------------------*/
void
tokenizer_error_print(void)
{
  OUTDEV.print(F("TKErr: "));
  OUTDEV.println(tokenizer_token());
  OUTDEV.println(BasProg.pChar);
}
/*---------------------------------------------------------------------------*/
uint8_t
tokenizer_finished(void){
  return *BasProg.pChar == 0 || BasProg.CurrentToken == TOKENIZER_ENDOFINPUT;
}
/*---------------------------------------------------------------------------*/
uint8_t
tokenizer_variable_num(void)
{
  return *BasProg.pChar - 'A';
}
/*---------------------------------------------------------------------------*/


