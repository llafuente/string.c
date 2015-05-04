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

TASK_IMPL(alloc) {
  //
  // memory
  //
  string* s = st_new(2, st_enc_ascii);
  string* s2;

  ASSERT(s->used == 0, "string used");
  ASSERT(s->length == 0, "string length");
  ASSERT(s->capacity == 3, "string capacity"); // 3 because is null-terminated
  ASSERT(s->value[0] == '\0', "string value");

  // ASSERT(is_utf8(T_STR_02) == 0);
  // ASSERT(is_utf8(T_STR_03_REP4) == 0);

  // st_resize(&s, 50);
  st_copyc(&s, T_STR_01, st_enc_ascii);
  ASSERT(s->capacity == 13, "string T_STR_01 capacity");
  ASSERT_STR(s, T_STR_01, st_enc_ascii);

  st_copyc(&s, T_STR_02, st_enc_ascii);
  ASSERT_STR(s, T_STR_02, st_enc_ascii);

  st_copyc(&s, T_STR_03, st_enc_utf8);
  ASSERT_STR(s, T_STR_03, st_enc_utf8);
  st_delete(&s);

  s = st_newc(T_STR_03, st_enc_utf8);
  ASSERT_STR(s, T_STR_03, st_enc_utf8);

  s2 = st_clone(s);
  ASSERT_STR(s2, T_STR_03, st_enc_utf8);
  ASSERT(s->capacity == s2->capacity, "s & s2 capacity must be the same");
  st_delete(&s2);

  s2 = st_rclone(s, 128);
  ASSERT_STR(s2, T_STR_03, st_enc_utf8);
  ASSERT(s2->capacity == 129, "s capacity 129");
  st_delete(&s2);

  s2 = st_new(1, st_enc_utf8);
  st_copy(&s2, s);
  ASSERT_STR(s2, T_STR_03, st_enc_utf8);

  st_clear(s2);
  ASSERT_STR(s2, "", st_enc_utf8);

  st_delete(&s2);

  st_delete(&s);

  s = st_newc("012345", st_enc_ascii);
  char* d = st_dump(s);
  ASSERT(strcmp(d, XDUMP_STR_012345) == 0, "dump ok");
  // test dump can be used
  s2 = (string*)DUMP_STR_012345;
  ASSERT_STR(s2, s->value, st_enc_ascii);
  st_delete(&s);
  free(d);

  return 0;
}
