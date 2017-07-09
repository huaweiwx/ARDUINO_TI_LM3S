/*
 * Copyright (c) 2006, Adam Dunkels
                 2013  huawei huaweiwx@wxmedi.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
 //ver hw150804 for arduino

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

//types
#define TOKENIZER_ERROR      0
#define TOKENIZER_ENDOFINPUT 1
#define TOKENIZER_NUMBER     2
#define TOKENIZER_HEXNUMBER  3
#define TOKENIZER_STRING     4
#define TOKENIZER_VARIABLE   5
#define TOKENIZER_FUNC       6

//commands
#define TOKENIZER_COM_START 	             10
#define TOKENIZER_LET     TOKENIZER_COM_START+0
#define TOKENIZER_PRINT   TOKENIZER_COM_START+1
#define TOKENIZER_IF      TOKENIZER_COM_START+2
#define TOKENIZER_THEN    TOKENIZER_COM_START+3
#define TOKENIZER_ELSE    TOKENIZER_COM_START+4
#define TOKENIZER_FOR     TOKENIZER_COM_START+5
#define TOKENIZER_TO      TOKENIZER_COM_START+6
#define TOKENIZER_NEXT    TOKENIZER_COM_START+7
#define TOKENIZER_GOTO    TOKENIZER_COM_START+8
#define TOKENIZER_GOSUB   TOKENIZER_COM_START+9
#define TOKENIZER_RETURN  TOKENIZER_COM_START+10
#define TOKENIZER_END     TOKENIZER_COM_START+11
#define TOKENIZER_RUN     TOKENIZER_COM_START+12

#define TOKENIZER_SHIFTOUT      TOKENIZER_COM_START+13
#define TOKENIZER_TONE          TOKENIZER_COM_START+14
#define TOKENIZER_NOTONE        TOKENIZER_COM_START+15


#define TOKENIZER_CONTINU TOKENIZER_COM_START+20   //30
#define TOKENIZER_LIST   TOKENIZER_CONTINU+1
#define TOKENIZER_LOAD   TOKENIZER_CONTINU+2
#define TOKENIZER_SAVE   TOKENIZER_CONTINU+3
#define TOKENIZER_CLS    TOKENIZER_CONTINU+4       //lcd use   
#define TOKENIZER_NEW    TOKENIZER_CONTINU+5
#define TOKENIZER_HELP   TOKENIZER_CONTINU+6
#define TOKENIZER_EEWR   TOKENIZER_CONTINU+7
#define TOKENIZER_EPWR   TOKENIZER_CONTINU+8
#define TOKENIZER_LCD    TOKENIZER_CONTINU+9       //lcd use
#define TOKENIZER_ON     TOKENIZER_CONTINU+10
#define TOKENIZER_OFF    TOKENIZER_CONTINU+11
#define TOKENIZER_DELAYMS  TOKENIZER_CONTINU+12
#define TOKENIZER_DELAYUS  TOKENIZER_CONTINU+13
#define TOKENIZER_CSR    TOKENIZER_CONTINU+14      //lcd use
#define TOKENIZER_DIGITALTOGGLE     TOKENIZER_CONTINU+15
#define TOKENIZER_PINMODE     TOKENIZER_CONTINU+16
#define TOKENIZER_DIGITALWRITE	TOKENIZER_CONTINU+17
#define TOKENIZER_ANALOGWRITE	TOKENIZER_CONTINU+18

#define TOKENIZER_OPEN   TOKENIZER_CONTINU+19
#define TOKENIZER_DIR  TOKENIZER_CONTINU+20
#define TOKENIZER_SYS    TOKENIZER_CONTINU+21
#define TOKENIZER_DO     TOKENIZER_CONTINU+22
#define TOKENIZER_WHILE  TOKENIZER_CONTINU+23
#define TOKENIZER_UNTIL  TOKENIZER_CONTINU+24
#define TOKENIZER_RESET  TOKENIZER_CONTINU+25
#define TOKENIZER_REM    TOKENIZER_CONTINU+26
//#define TOKENIZER_CALL   TOKENIZER_CONTINU+27  


//oprates
#define TOKENIZER_OPRATE_START 	TOKENIZER_CONTINU+30       //30+30=60
#define TOKENIZER_AT            TOKENIZER_OPRATE_START+1   // @
#define TOKENIZER_LT 			TOKENIZER_OPRATE_START+2        // >
#define TOKENIZER_GT 			TOKENIZER_OPRATE_START+3        // <
#define TOKENIZER_AND           TOKENIZER_OPRATE_START+4   // &

#define TOKENIZER_CRLF 			TOKENIZER_OPRATE_START+5
#define TOKENIZER_CR 			TOKENIZER_OPRATE_START+5

#define TOKENIZER_COMMA 		TOKENIZER_OPRATE_START+6     // ,
#define TOKENIZER_SEMICOLON 	TOKENIZER_OPRATE_START+7     // ; 
#define TOKENIZER_PLUS 			TOKENIZER_OPRATE_START+8     // +
#define TOKENIZER_MINUS 		TOKENIZER_OPRATE_START+9     // -
#define TOKENIZER_ASTR 			TOKENIZER_OPRATE_START+10    // *
#define TOKENIZER_COLON 	    TOKENIZER_OPRATE_START+11    // :
#define TOKENIZER_SLASH		 	TOKENIZER_OPRATE_START+12    // '/'
#define TOKENIZER_MOD 			TOKENIZER_OPRATE_START+13    // %
#define TOKENIZER_EQ 			TOKENIZER_OPRATE_START+14    // =
#define TOKENIZER_OR  			TOKENIZER_OPRATE_START+15    // |
#define TOKENIZER_LEFTPAREN 	TOKENIZER_OPRATE_START+16    //(
#define TOKENIZER_RIGHTPAREN 	TOKENIZER_OPRATE_START+17    //)
#define TOKENIZER_DOLLARS 		TOKENIZER_OPRATE_START+18    //$
#define TOKENIZER_IO 	      	TOKENIZER_OPRATE_START+19    //!
#define TOKENIZER_POWER     	TOKENIZER_OPRATE_START+20    //^
#define TOKENIZER_CLOSE     	TOKENIZER_OPRATE_START+21    //#
#define TOKENIZER_LEFTSPAREN 	TOKENIZER_OPRATE_START+22    //[
#define TOKENIZER_RIGHTSPAREN 	TOKENIZER_OPRATE_START+23    //]



//device
#define TOKENIZER_DEVICE_START 	TOKENIZER_OPRATE_START+30  // 60+30=90

#define TOKENIZER_EEP           TOKENIZER_DEVICE_START+0  //eeprom
#define TOKENIZER_FLA           TOKENIZER_DEVICE_START+1  //flash prom
#define TOKENIZER_COM           TOKENIZER_DEVICE_START+2  //uart
#define TOKENIZER_SET           TOKENIZER_DEVICE_START+3  //set

//events & port
#define TOKENIZER_TIMER         TOKENIZER_DEVICE_START+4
#define TOKENIZER_INTE          TOKENIZER_DEVICE_START+5
#define TOKENIZER_IIC           TOKENIZER_DEVICE_START+6
#define TOKENIZER_SPI           TOKENIZER_DEVICE_START+7
#define TOKENIZER_PIN           TOKENIZER_DEVICE_START+8
#define TOKENIZER_IRET          TOKENIZER_DEVICE_START+9

//func
#define FUNC_START 	        TOKENIZER_DEVICE_START+10  //90+10=100
#define FUNC_EERD			FUNC_START+1
#define FUNC_EPRD 			FUNC_START+2
#define FUNC_PEEK 			FUNC_START+3
#define FUNC_INFO			FUNC_START+4
#define FUNC_GETBIT			FUNC_START+6

//#define FUNC_PORTR			FUNC_START+7  //port input read
#define FUNC_ANALOGREAD		FUNC_START+8  //pin analog read
#define FUNC_DIGITALREAD	FUNC_START+9  //pin bit read
#define FUNC_MILLIS			FUNC_START+10
#define FUNC_MICROS			FUNC_START+11
#define FUNC_SHIFTIN		FUNC_START+12
#define FUNC_PULSEIN		FUNC_START+13
#define FUNC_ABS		    FUNC_START+14
#define FUNC_INPUT		    FUNC_START+15

/************************************************
用途：Contrul basic ERR No. 定义
*************************************************/

//sys
#define ERR_NONE          0

#define ERR_ACCEPT 	      10
#define ERR_FUN_NONE      ERR_ACCEPT+1
#define ERR_FACTOR        ERR_ACCEPT+2
#define ERR_VARNUM        ERR_ACCEPT+3
#define ERR_STATEMENT     ERR_ACCEPT+4
#define ERR_RELATION      ERR_ACCEPT+5
#define ERR_TERM          ERR_ACCEPT+6
#define ERR_SAVE_DEV      ERR_ACCEPT+7
#define ERR_LOAD_DEV      ERR_ACCEPT+8
#define ERR_LOAD_EEP      ERR_ACCEPT+9

#define ERR_FOR           ERR_ACCEPT+10  //20
#define ERR_ON            ERR_FOR+1
#define ERR_FOR_EQ        ERR_FOR+2
#define ERR_FOR_TO        ERR_FOR+3
#define ERR_FOR_STACK     ERR_FOR+4
#define ERR_NEXT_STACK    ERR_FOR+5
#define ERR_SYS           ERR_FOR+6
#define ERR_SET           ERR_FOR+7
#define ERR_DELAY         ERR_FOR+8
#define ERR_ARGS          ERR_FOR+9

#define ERR_FUNC          ERR_FOR+10   //30
#define ERR_INFO          ERR_FUNC+1

#define ERR_GOSUB_NUM     ERR_FUNC+2
#define ERR_GOSUB_STACK   ERR_FUNC+3
#define ERR_RETURN_STACK  ERR_FUNC+4
#define ERR_LET_EQ        ERR_FUNC+5
#define ERR_LCD           ERR_FUNC+6
#define ERR_EEWR          ERR_FUNC+7


#define ERR_EXPR          ERR_FUNC+8 
#define ERR_NO_LEFTPAREN  ERR_FUNC+9
#define ERR_NO_RIGHTPAREN ERR_FUNC+10
#define ERR_NO_COLON      ERR_FUNC+11

#define ERR_DO            ERR_FUNC+12
#define ERR_DO_STACK      ERR_FUNC+13




#define ERR_EVENTS_START  ERR_FUNC+20  //50
#define ERR_TIMER         ERR_EVENTS_START+0
#define ERR_TIMER_NUM     ERR_EVENTS_START+1
#define ERR_INTE          ERR_EVENTS_START+2
#define ERR_INTE_NUM      ERR_EVENTS_START+3
#define ERR_IRET_ADR      ERR_EVENTS_START+4
#define ERR_SET81         ERR_EVENTS_START+10


void tokenizer_init(const char *program);
void tokenizer_next(void);
uint8_t tokenizer_token(void);
uint8_t tokenizer_func_token(void);
int32_t tokenizer_num(void);
uint8_t tokenizer_char(void);
int32_t tokenizer_hexnum(void);
uint8_t tokenizer_variable_num(void);
void tokenizer_string(char *dest, int len);

uint8_t tokenizer_finished(void);
void tokenizer_error_print(void);

#endif /* __TOKENIZER_H__ */
