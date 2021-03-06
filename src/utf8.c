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

st_len_t st_utf8_length(const char* src, st_size_t* bytes) {
  st_len_t len = 0;
  const char* p = src;
  char jump;
  st_size_t used_bytes = 0;

  while (*p != '\0') {
    // printf("%c @%p %d\n", *p, p, utf8_next(p));
    jump = st_utf8_char_size(p);
    p += jump;
    used_bytes += jump;
    // printf("%c\n", *p);
    ++len;
  }
  //++used_bytes; do not include '\0'!

  if (bytes) {
    *bytes = used_bytes;
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

st_uc4_t st_utf8_codepoint(const char* utf8) {
  st_uc_t* p = (st_uc_t*)utf8;
  st_uc_t c = *p;

  if ((c < (st_uc_t)'\200')) {
    return c;
  }

  if ((c >= (st_uc_t)'\370')) {
    return 0;
  }

  if ((c >= (st_uc_t)'\360')) {
    return ((((262144 * (c % 8)) + (4096 * (p[1] % 64))) + (64 * (p[2] % 64))) +
            (p[3] % 64));
  }

  if ((c >= (st_uc_t)'\340')) {
    return (((4096 * (c % 16)) + (64 * (p[1] % 64))) + (p[2] % 64));
  }

  if ((c >= (st_uc_t)'\300')) {
    return ((64 * (c % 32)) + (p[1] % 64));
  }

  return 0;
}

// utf32 code point to utf8 chars
st_len_t st_utf8_from_codepoint(char* utf8, st_uc4_t codepoint) {
  st_uc_t* p = (st_uc_t*)utf8;
  if (codepoint < 128) {
    p[0] = codepoint;
    return 1;
  }

  if (codepoint < 2048) {
    p[1] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[0] = (192 + (codepoint % 32));
    return 2;
  }

  if (codepoint < 65536) {
    p[2] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[1] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[0] = (224 + codepoint);
    return 3;
  }

  if (codepoint < 1048576) {
    p[3] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[2] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[1] = (128 + (codepoint % 64));
    codepoint = (codepoint / 64);
    p[0] = (240 + codepoint);
    return 4;
  }
  return 0; // err
}
