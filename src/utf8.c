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

size_t st_utf8_length(const char* src, size_t* used_bytes) {
  size_t len = 0;
  const char* p = src;
  char jump;
  size_t capacity = 0;

  while (*p != '\0') {
    // printf("%c @%p %d\n", *p, p, utf8_next(p));
    jump = st_utf8_char_size(*p);
    p += jump;
    capacity += jump;
    // printf("%c\n", *p);
    ++len;
  }
  //++capacity; do not include '\0'!

  if (used_bytes) {
    *used_bytes = capacity;
  }

  return len;
}

int st_utf8_char_size_safe(unsigned char lead_chr) {
  if (lead_chr <= 0x7F) {
    return 1;
  } else if (lead_chr >= 0xC0 /*11000000*/ && lead_chr <= 0xDF /*11011111*/) {
    return 2;
  } else if (lead_chr >= 0xE0 /*11100000*/ && lead_chr <= 0xEF /*11101111*/) {
    return 3;
  } else if (lead_chr >= 0xF0 /*11110000*/ &&
             lead_chr <= 0xF4 /* Cause of RFC 3629 */) {
    return 4;
  }
  // invalid!
  return -1;
}

char* st_utf8_invalid(const unsigned char* str, size_t len) {
  size_t i = 0;
  int continuation_bytes = 0;
  unsigned char cache;
  const unsigned char* end = str + len;

  while (str < end) {
    cache = *str;

    continuation_bytes = st_utf8_char_size_safe(cache) - 1;

    if (continuation_bytes < 0) {
      return (char*)str;
    }

    ++str;
    while (i < len && continuation_bytes > 0 && (cache = *str) >= 0x80 &&
           cache <= 0xBF) {
      ++str;
      continuation_bytes -= 1;
    }

    if (continuation_bytes != 0) {
      return (char*)str;
    }
  }

  return 0;
}

// TODO i'm almost sure that this could be improved with a sum == X
// same technique used in st_utf8_char_size_safe
bool st_utf8_char_eq(char* a, char* b) {
  st_uc_t ac = (st_uc_t)*a;
  st_uc_t bc = (st_uc_t)*b;

  if (ac != bc) {
    return false;
  }

  if (ac >= 0xc0 && *(a + 1) != *(b + 1)) {
    return false;
  }

  if (ac >= 0xe0 && *(a + 2) != *(b + 2)) {
    return false;
  }

  if (ac >= 0xf0 && *(a + 3) != *(b + 3)) {
    return false;
  }

  return true;
}

st_len_t st_utf8_char_size(st_uc_t byte) {
  return 1 + (((byte) >= 0xc0) + ((byte) >= 0xe0) + ((byte) >= 0xf0));
}
