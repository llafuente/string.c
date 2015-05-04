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

TASK_IMPL(internal) {
  string* s;
  char* start;
  char* end;

  st_len_t start_pos = 0;
  st_len_t end_pos = 0;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 0);
  assert(end_pos == 10);

  start_pos = 1;
  end_pos = -1;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 1);
  assert(end_pos == 8);

  start_pos = 5;
  end_pos = 5;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 5);
  assert(end_pos == 5);

  start_pos = 9;
  end_pos = 0;
  st__calc_range(21, &start_pos, &end_pos);
  assert(start_pos == 9);
  assert(end_pos == 12);

  start_pos = -2;
  end_pos = 2;
  st__calc_range(21, &start_pos, &end_pos);
  assert(start_pos == 19);
  assert(end_pos == 2);

  s = st_newc("0123456789", st_enc_ascii);

  assert(st__get_char_offset(s, 0) == s->value);
  assert(st__get_char_offset(s, 5) - 5 == s->value);
  assert(st__get_char_offset(s, 3) - 3 == s->value);
  assert(st__get_char_offset(s, 9) - 9 == s->value);

  assert(st__get_char_offset(s, -5) - 5 == s->value);
  assert(st__get_char_offset(s, -9) - 1 == s->value);
  assert(st__get_char_offset(s, -1) - 9 == s->value);
  /*
  st__get_char_range(s, 0, 5, &start, &end);
  assert(start == s->value);
  assert(end == s->value + 5);


  st__get_char_range(s, -2, 2, &start, &end);
  assert(start == s->value + 8);
  assert(end == s->value + 10);


  st__get_char_range(s, -2, -1, &start, &end);
  assert(start == s->value + 8);
  assert(end == s->value + 9);
  */

  st_delete(&s);

  // st__mempbrk

  // Check 1
  string* x;
  string* y;
  char* ret;
  x = st_newc("I will not have my fwends widiculed by the common soldiewy",
              st_enc_ascii);
  y = st_newc("zpm1", st_enc_ascii); // Finds the 'm'
  ret = st__mempbrk(x->value, y->value);
  assert(ret == &x->value[16]); //, "Simple strpbrk()" );
  st_delete(&x);
  st_delete(&y);

  // Check 2
  x = st_newc("Not bad for a little fur ball. You! Stay here.", st_enc_ascii);
  y = st_newc("zx", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == NULL); //, "Letters not found");
  st_delete(&x);
  st_delete(&y);

  // Check 3 (boundary condition)
  x = st_newc("", st_enc_ascii);
  y = st_newc("zx", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "String to search empty" );
  st_delete(&x);
  st_delete(&y);

  // Check 4 (boundary condition)
  x = st_newc("zx", st_enc_ascii);
  y = st_newc("", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "Empty search string" );
  st_delete(&x);
  st_delete(&y);

  // Check 5 (boundary condition)
  x = st_newc("", st_enc_ascii);
  y = st_newc("", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "Both strings empty" );
  st_delete(&x);
  st_delete(&y);

  return 0;
}
