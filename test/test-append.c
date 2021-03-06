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

TASK_IMPL(append) {
  string* s = st_newc(T_STR_03, st_enc_utf8);

  st_append(&s, s);
  ASSERT_STR(s, T_STR_03_REP2, st_enc_utf8);

  st_append(&s, s);
  ASSERT_STR(s, T_STR_03_REP4, st_enc_utf8);

  st_delete(&s);

  s = st_newc(T_STR_03, st_enc_utf8);
  string* aux = st_concat(s, s);
  ASSERT_STR(aux, T_STR_03_REP2, st_enc_utf8);

  st_delete(&aux);
  st_delete(&s);

  s = st_newc("wtf", st_enc_utf8);

  st_append_char(&s, '0');
  ASSERT_STR(s, "wtf0", st_enc_utf8);

  st_delete(&aux);
  st_delete(&s);

  s = st_newc("wtf", st_enc_utf8);

  aux = st_concat_random(s, 10);
  ASSERT(aux->length == s->length + 10, "random size ok");

  st_delete(&aux);
  st_delete(&s);

  return 0;
}
