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

TASK_IMPL(case) {
  string* s = st_newc("abc", st_enc_utf8);
  string* up = st_upper(s);
  ASSERT_STR(up, "ABC", st_enc_utf8);

  st_delete(&s);
  st_delete(&up);

  s = st_newc("áéíóú", st_enc_utf8);

  up = st_ucfirst(s);
  ASSERT_STR(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  up = st_upper(s);
  ASSERT_STR(up, "ÁÉÍÓÚ", st_enc_utf8);
  st_delete(&up);

  up = st_capitalize(s);
  ASSERT_STR(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  st_delete(&s);

  s = st_newc("ÁÉÍÓÚ", st_enc_utf8);

  up = st_capitalize(s);
  ASSERT_STR(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  up = st_ucfirst(s);
  ASSERT_STR(up, "ÁÉÍÓÚ", st_enc_utf8);
  st_delete(&up);

  st_delete(&s);

  char buffer[12];
  st__char_upper("a", buffer, st_enc_ascii);
  ASSERT(strcmp(buffer, "A") == 0, "a uppercased");

  st__char_lower((char*)L"A", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"a") == 0, "a utf32 uppercased");

  st__char_lower((char*)L"Â", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"â") == 0, "ä utf32 uppercased");

  st__char_lower((char*)L"亜", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"亜") == 0, "? utf32 uppercased");

  st__char_upper((char*)L"ｅ", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"Ｅ") == 0, "? utf32 uppercased");

  st__char_lower((char*)L"Ｅ", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"ｅ") == 0, "? utf32 uppercased");

  return 0;
}
