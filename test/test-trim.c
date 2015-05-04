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

TASK_IMPL(trim) {
  string* s;
  string* result;
  string* mask;

  s = st_newc("   123  ", st_enc_ascii);
  result = st_trim(s, 0, 3);

  ASSERT_STR(result, "123", st_enc_ascii);

  st_delete(&s);
  st_delete(&result);

  mask = st_newc("0", st_enc_ascii);
  s = st_newc("000123x0", st_enc_ascii);

  result = st_trim(s, mask, 3);
  ASSERT(strcmp(result->value, "123x") == 0, "trim('000123x0') = '123x'");
  st_delete(&result);

  result = st_chop(s, mask);
  ASSERT(strcmp(result->value, "000123x") == 0, "chop('000123x0') = '000123x'");
  st_delete(&result);

  result = st_ltrim(s, mask);
  ASSERT(strcmp(result->value, "123x0") == 0, "ltrim('000123x0') = '123x0'");
  st_delete(&result);
  st_delete(&mask);

  mask = st_newc("9", st_enc_ascii);
  result = st_rtrim(s, mask);
  ASSERT(strcmp(result->value, "000123x0") == 0,
         "rtrim('000123x0') = '000123x0'");
  st_delete(&result);

  st_delete(&mask);
  st_delete(&s);

  return 0;
}
