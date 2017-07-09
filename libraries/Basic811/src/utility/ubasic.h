/* Copyright (c) 2006, Adam Dunkels
                 2013, huawei huaweiwx@sina.com
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

#ifndef __UBASIC_H__
#define __UBASIC_H__

void ubasic_setup(void);
void ubasic_init(const char *program);
void ubasic_run(void);
uint8_t  ubasic_finished(void);
int32_t  function_calcul(void);
void accept(uint8_t token);
void list_statement(void);
int32_t expr(void);
int32_t ubasic_get_variable(uint8_t varnum);
void ubasic_set_variable(uint8_t varum, int32_t value);
void statement(void);
void load_statement(void);
void save_statement(void);
void dir_statement(void);
//array
int16_t isArray(void);
void    arrayWrite(uint8_t,uint16_t,int16_t);
int32_t arrayRead(uint8_t varnum,uint16_t adr);
#endif /* __UBASIC_H__ */
