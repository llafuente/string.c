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

/// @file

#include "stringc.h"

string* string_sub(string* str, int start, int end) {
  assert(end < str->length);
  assert(end > -str->length);

  size_t len = 0;
  if (start < 0) {

    len -= start;
    len += end;
  } else {
    len = end - start;
  }

  //printf("string %s start %d end %d\n", str->value, start, end);
  //printf("len %zu\n", len);

  string* out = string_new(len);

  size_t pos,
  idx = 0;
  while (start < 0) {
    pos = str->length + start;
    //printf("pos %zu", pos);
    out->value[idx++] = str->value[pos];

    ++start;
  }

  //printf("start %d %c\n", start, str->value[start]);
  //printf("idx %zu\n", idx);

  while (start != end) {
    out->value[idx++] = str->value[start++];
  }
  out->length = idx;

  //printf("idx %zu\n", idx);

  string_zeronull(out);

  return out;
}
