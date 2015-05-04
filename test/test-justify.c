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

TASK_IMPL(justify) {
  string* x;
  string* z;

  x = st_newc("abc", st_enc_ascii);

  z = st_center(x, 5, 0);
  ASSERT_STR(z, " abc ", st_enc_ascii);
  st_delete(&z);

  z = st_left(x, 5, 0);
  ASSERT_STR(z, "abc  ", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 5, 0);
  ASSERT_STR(z, "  abc", st_enc_ascii);
  st_delete(&z);

  z = st_center(x, 6, 0);
  ASSERT_STR(z, " abc  ", st_enc_ascii);
  st_delete(&z);

  z = st_left(x, 6, 0);
  ASSERT_STR(z, "abc   ", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 6, 0);
  ASSERT_STR(z, "   abc", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 2, 0);
  ASSERT_STR(z, "abc", st_enc_ascii);
  st_delete(&z);

  string* padstr = st_newc("x", st_enc_ascii);
  z = st_right(x, 6, padstr);
  ASSERT_STR(z, "xxxabc", st_enc_ascii);
  st_delete(&z);
  st_delete(&padstr);
  st_delete(&x);

  x = st_newc("是龍", st_enc_utf8);
  padstr = st_newc("☃", st_enc_utf8);

  z = st_right(x, 10, padstr);
  ASSERT_STR(z, "☃☃☃☃☃☃☃☃是龍", st_enc_utf8);
  st_delete(&z);
  st_delete(&padstr);
  st_delete(&x);

  return 0;
}
