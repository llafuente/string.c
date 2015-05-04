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

TASK_IMPL(ascii) {
  assert(st_ascii_char_size_safe("\xFF") == -1);
  assert(st_ascii_char_size_safe("x") == 1);
  assert(st_ascii_char_size_safe("z") == 1);

  ASSERT(st_is_ascii(T_STR_ASCII) == true, "ascii is ascii -> true!");
  ASSERT(st_is_ascii(T_STR_03) == false, "utf8 is ascii -> false!");

  ASSERT(st_ascii_valid_codepoint(5) == true, "in range codepoint")
  ASSERT(st_ascii_valid_codepoint(300) == false, "out of range codepoint");

  st_size_t bytes;
  ASSERT(st_ascii_length("abc", &bytes) == 3, "length 3");
  ASSERT(bytes == 3, "size 3");
  ASSERT(st_ascii_char_size("a") == 1, "ascii char size is always 1");

  return 0;
}
