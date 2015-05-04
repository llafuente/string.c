/*
* Copyright 2015 Luis Lafuente <llafuente@noboxout.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stringc.h"
#include "fixtures.h"
#include "tasks.h"

TASK_IMPL(from) {
  // st_base2number
  string* s = st_newc(T_STR_5BIN, st_enc_ascii);
  int d = st_base2number(s, 2);
  ASSERT(d == 5, "st_base2number is 5");
  st_delete(&s);

  s = st_dec2hex(10);
  ASSERT_STR(s, "a", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex(47);
  ASSERT_STR(s, "2f", st_enc_ascii);
  st_delete(&s);

  s = st_newc("111000111", st_enc_ascii);
  double dd = st_bin2dec(s);
  ASSERT(dd == 455, "bin2dec(111000111) == 455");
  st_delete(&s);

  s = st_newc("0111000111", st_enc_ascii);
  dd = st_bin2dec(s);
  ASSERT(dd == 455, "bin2dec(0111000111) == 0");
  st_delete(&s);

  s = st_newc("01234567", st_enc_ascii);
  dd = st_oct2dec(s);
  ASSERT(dd == 342391, "oct2dec(01234567) == 342391");
  st_delete(&s);

  s = st_newc("0567", st_enc_ascii);
  dd = st_oct2dec(s);
  ASSERT(dd == 375, "oct2dec(0567) == 375");
  st_delete(&s);

  s = st_newc("123abc", st_enc_ascii);
  dd = st_hex2dec(s);
  ASSERT(dd == 1194684, "hex2dec(123abc) == 1194684");
  st_delete(&s);

  s = st_newc("789DEF", st_enc_ascii);
  dd = st_hex2dec(s);
  ASSERT(dd == 7904751, "hex2dec(789DEF) == 7904751");
  st_delete(&s);

  s = st_dec2bin(10);
  ASSERT_STR(s, "1010", st_enc_ascii);
  st_delete(&s);

  s = st_dec2bin((size_t)3.9505e3);
  ASSERT_STR(s, "111101101110", st_enc_ascii);
  st_delete(&s);

  s = st_dec2oct(10);
  ASSERT_STR(s, "12", st_enc_ascii);
  st_delete(&s);

  s = st_dec2oct((size_t)3950.5);
  ASSERT_STR(s, "7556", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex(10);
  ASSERT_STR(s, "a", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex((size_t)3950.5);
  ASSERT_STR(s, "f6e", st_enc_ascii);
  st_delete(&s);

  // st_number2base

  string* binstr = st_number2base(5, 2);
  ASSERT_STR(binstr, T_STR_5BIN, st_enc_ascii);
  st_delete(&binstr);

  return 0;
}
