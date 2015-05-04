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

TASK_IMPL(compare) {
  string* s = st_newc(T_STR_CMP1, st_enc_ascii);
  string* aux = st_newc(T_STR_CMP2, st_enc_ascii);

  ASSERT(st_compare(s, s) == 0, "cmp same string");
  ASSERT(st_compare(aux, aux) == 0, "cmp same string");
  ASSERT(st_compare(s, aux) > 0, "cmp T_STR_CMP1 vs T_STR_CMP2");
  ASSERT(st_compare(aux, s) < 0, "cmp T_STR_CMP2 vs T_STR_CMP1");

  st_copyc(&aux, T_STR_CMP3, st_enc_ascii);
  ASSERT(st_cmp(s, aux) < 0, "cmp T_STR_CMP1 vs T_STR_CMP3");
  ASSERT(st_cmp(aux, s) > 0, "cmp T_STR_CMP3 vs T_STR_CMP1");

  st_delete(&aux);
  st_delete(&s);

  s = st_newc("123", st_enc_ascii);
  aux = st_newc("1", st_enc_ascii);
  ASSERT(st_compare(s, aux) > 0, "cmp 123 vs 1");
  ASSERT(st_compare(aux, s) < 0, "cmp 1 vs 123");
  st_delete(&aux);
  st_delete(&s);

  s = st_newc("ab01cd", st_enc_ascii);
  aux = st_newc("01", st_enc_ascii);

  ASSERT(st_subcompare(s, aux, 2, 2) == 0, "substring compare");
  ASSERT(st_subcompare(s, aux, 2, 3) == 1, "substring compare");
  ASSERT(st_scmp(s, aux, 2, 1) == -1, "substring compare");
  ASSERT(st_scmp(s, aux, 3, 2) == 1, "substring compare");

  st_delete(&aux);
  st_delete(&s);

  return 0;
}
