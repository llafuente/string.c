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

st_len_t st_utf8_length(const char* src, size_t* capacity) {
  st_len_t len = 0;
  const char* p = src;
  char jump;
  size_t used_bytes = 0;

  while (*p != '\0') {
    // printf("%c @%p %d\n", *p, p, utf8_next(p));
    jump = st_utf8_char_size(p);
    p += jump;
    used_bytes += jump;
    // printf("%c\n", *p);
    ++len;
  }
  //++used_bytes; do not include '\0'!

  if (capacity) {
    *capacity = used_bytes;
  }

  return len;
}

st_len_t st_utf8_char_size_safe(const char* input) {
  st_uc_t lead_chr = (st_uc_t)*input;

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

char* st_utf8_invalid(const st_uc_t* str, size_t len) {
  size_t i = 0;
  int continuation_bytes = 0;
  st_uc_t cache;
  const st_uc_t* end = str + len;

  while (str < end) {
    continuation_bytes = st_utf8_char_size_safe((const char*)str) - 1;

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

st_len_t st_utf8_char_size(const char* input) {
  return st_utf8_lead_size((st_uc_t)*input);
}

st_len_t st_utf8_lead_size(st_uc_t lead) {
  return 1 + (((lead) >= 0xc0) + ((lead) >= 0xe0) + ((lead) >= 0xf0));
}

bool st_utf8_valid_codepoint(st_uc4_t uc_cp) {
  if (uc_cp >= 0x110000 || ((uc_cp & 0xFFFF) >= 0xFFFE) ||
      (uc_cp >= 0xD800 && uc_cp < 0xE000) ||
      (uc_cp >= 0xFDD0 && uc_cp < 0xFDF0)) {
    return false;
  }
  return true;
}
