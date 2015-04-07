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
//TODO test performance using (also if are compatible)
/*
if ((code & 0xffffff80) == 0) return 1;
else if ((code & 0xfffff800) == 0) return 2;
else if ((code & 0xffff0000) == 0) return 3;
else if ((code & 0xffe00000) == 0) return 4;
else if ((code & 0xfc000000) == 0) return 5;
else if ((code & 0x80000000) == 0) return 6;
*/

const char string_utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};
const char * const string_utf8_skip = string_utf8_skip_data;

size_t string_utf8_lenc(const char* src, size_t *out_capacity) {
  size_t len = 0;
  const char *p = src;
  char jump;
  size_t capacity = 0;

  while (*p != '\0') {
    //printf("%c @%p %d\n", *p, p, utf8_next(p));
    jump = string_utf8_jump_next(p);
    p += jump;
    capacity += jump;
    //printf("%c\n", *p);
    ++len;
  }
  ++capacity;

  if (out_capacity) {
    *out_capacity = capacity;
  }

  return len;
}

int string_utf8_next_pos(unsigned char chr) {
  if (chr <= 0x7F) {
    return 0;
  } else if (chr >= 0xC0 /*11000000*/ && chr <= 0xDF /*11011111*/) {
    return 1;
  } else if (chr >= 0xE0 /*11100000*/ && chr <= 0xEF /*11101111*/) {
    return 2;
  } else if (chr >= 0xF0 /*11110000*/ && chr <= 0xF4 /* Cause of RFC 3629 */) {
    return 3;
  }
  // invalid!
  return -1;
}

char* string_utf8_invalid(const unsigned char *str, size_t len) {
  size_t i = 0;
  int continuation_bytes = 0;
  unsigned char cache;
  const unsigned char *end = str + len;

  while (str < end) {
    cache = *str;

    continuation_bytes = string_utf8_next_pos(cache);

    if (continuation_bytes < 0) {
      return (char*) str;
    }

    ++str;
    while (i < len && continuation_bytes > 0
      && (cache = *str) >= 0x80
      && cache <= 0xBF) {
      ++str;
      continuation_bytes -= 1;
    }

    if (continuation_bytes != 0) {
      return (char*) str;
    }
  }

  return 0;
}
