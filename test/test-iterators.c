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

char buffer[256];
void char_itr_cb(const string* chr, st_len_t pos, string* src) {
  strcat(buffer, chr->value);
}

void wchar_itr_cb(const string* chr, st_len_t pos, string* src) {
  st_hexdump(chr->value, 8);
  wcscat((wchar_t*)buffer, (wchar_t*)chr->value);
}

void byte_itr_cb(st_uc_t chr, st_len_t pos, const string* src) {
  st_len_t i = strlen(buffer);
  buffer[i] = (char)chr;
  buffer[i + 1] = '\0';
}

void char_map_cb(string* chr, st_len_t pos, string* src) { chr->value[0] += 1; }

TASK_IMPL(iterators) {
  buffer[0] = '\0';
  string* str;
  string* nstr;

  str = st_newc(T_STR_ASCII, st_enc_ascii);
  buffer[0] = '\0';
  st_char_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, T_STR_ASCII) == 0, "concat char iteration");
  st_delete(&str);

  str = st_newc(T_STR_03_REP3, st_enc_utf8);

  buffer[0] = '\0';
  st_char_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, T_STR_03_REP3) == 0, "concat char iteration");

  buffer[0] = '\0';
  st_byte_iterator(str, byte_itr_cb);
  ASSERT(strcmp(buffer, T_STR_03_REP3) == 0, "concat char iteration");

  st_delete(&str);

  buffer[0] = '\0';
  buffer[1] = '\0';
  buffer[2] = '\0';
  buffer[3] = '\0';
  str = st_newc((const char*)L"abcñ", st_enc_utf32be);
  st_char_iterator(str, (st_char_itr_cb)wchar_itr_cb);

  ASSERT(wcscmp((const wchar_t*)buffer, L"abcñ") == 0, "concat wide iteration");
  st_delete(&str);

  //
  // line iterator
  //
  str = st_newc("01\n2\n345\n\n67\n89", st_enc_utf8);

  buffer[0] = '\0';
  st_line_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, "0123456789") == 0,
         "concat line iteration (remove newlines)");

  st_delete(&str);

  str = st_newc("abc", st_enc_ascii);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  ASSERT_STR(nstr, "bcd", st_enc_ascii);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc("abc", st_enc_utf8);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  ASSERT_STR(nstr, "bcd", st_enc_utf8);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc(T_STR_03, st_enc_utf8);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  ASSERT_STR(nstr, "ojıp!㘃", st_enc_utf8);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc((const char*)L"ABC", st_enc_utf32be);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  ASSERT_STR(nstr, L"BCD", st_enc_utf32be);
  st_delete(&str);
  st_delete(&nstr);

  return 0;
}
