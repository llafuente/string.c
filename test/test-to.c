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

TASK_IMPL(to) {
  size_t num;
  string* s;
  string* s2;

  s = st_chr(97, st_enc_ascii);
  ASSERT_STR(s, "a", st_enc_ascii);
  assert(st_ord(s, 0) == 97);
  st_delete(&s);

  s = st_chr(98, st_enc_ascii);
  ASSERT_STR(s, "b", st_enc_ascii);
  assert(st_ord(s, 0) == 98);
  st_delete(&s);

  s = st_chr(65, st_enc_ascii);
  ASSERT_STR(s, "A", st_enc_ascii);
  assert(st_ord(s, 0) == 65);
  st_delete(&s);

  s = st_chr(65, st_enc_utf8);
  ASSERT_STR(s, "A", st_enc_utf8);
  assert(st_ord(s, 0) == 65);
  st_delete(&s);

  s = st_chr(40845, st_enc_utf8);
  ASSERT_STR(s, "龍", st_enc_utf8);
  assert(st_ord(s, 0) == 40845);
  st_delete(&s);

  s = st_chr(26159, st_enc_utf8);
  ASSERT_STR(s, "是", st_enc_utf8);
  assert(st_ord(s, 0) == 26159);
  st_delete(&s);

  // found: http://en.wikibooks.org/wiki/Unicode/Character_reference/2F000-2FFFF
  s = st_chr(195038, st_enc_utf8);
  ASSERT_STR(s, "軔", st_enc_utf8);
  assert(st_ord(s, 0) == 195038);
  st_delete(&s);

  /*
  s = st_chr(110011, st_enc_utf8);
  st_debug(s);
  st_delete(&s);
  s = st_newc("", st_enc_utf8);
  printf("%ld\n", st_ord(s, 0));
  exit(1);
  */

  s = st_newc(T_STR_03, st_enc_utf8);

  assert(st_ord(s, 0) == 110);
  assert(st_ord(s, 1) == 105);
  assert(st_ord(s, 2) == 241);
  assert(st_ord(s, 3) == 111);
  assert(st_ord(s, 4) == 32);
  assert(st_ord(s, 5) == 9731);
  // assert(st_ord(s, 6) == 0); out-of-range
  st_delete(&s);

  s = st_chr(9731, st_enc_utf8);
  ASSERT_STR(s, "☃", st_enc_utf8);
  st_delete(&s);

  s = st_chr(241, st_enc_utf8);
  ASSERT_STR(s, "ñ", st_enc_utf8);
  st_delete(&s);

  s = st_newc("\\n\\\\a", st_enc_utf8);
  s2 = st_unescape(s);

  ASSERT_STR(s2, "\n\\a", st_enc_utf8);
  st_delete(&s);
  st_delete(&s2);

  s = st_newc("\n\a abc \\xx", st_enc_utf8);
  s2 = st_escape(s);

  ASSERT_STR(s2, "\\n\\a abc \\\\xx", st_enc_utf8);
  st_delete(&s);
  st_delete(&s2);

  return 0;
}
