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

st_len_t st_utf32_length(const char* src, size_t* used_bytes) {
  st_len_t out = strlen(src);

  if (used_bytes) {
    *used_bytes = out;
  }

  return out;
}

bool st_utf32_valid_codepoint(st_uc4_t code_point) {
  // calculate the code point
  //
  // make sure the code point is within the valid range
  return code_point <= 0x10FFFF ? 4 : -1;
}

st_uc4_t st_utf32be_codepoint(const char* input) {
  return *(const st_uc4_t*)input;
}

st_uc4_t st_utf32le_codepoint(const char* input) {
  st_uc4_t a = *(const st_uc4_t*)input;

  return (a << 24) | ((a & 0x0000ff00UL) << 8) | ((a & 0x00ff0000UL) >> 8) |
         (a >> 24);
}

st_len_t st_utf32_char_size(const char* input) { return 4; }
st_len_t st_utf32_lead_size(st_uc_t lead_chr) { return 4; }

st_len_t st_utf32le_char_size_safe(const char* input) {
  if (st_utf32_valid_codepoint(st_utf32le_codepoint(input))) {
    return 4;
  }
  return -1;
}

st_len_t st_utf32be_char_size_safe(const char* input) {
  if (st_utf32_valid_codepoint(st_utf32be_codepoint(input))) {
    return 4;
  }
  return -1;
}

st_len_t st_utf32le_char_size(const char* input) { return 4; }

st_len_t st_utf32be_char_size(const char* input) { return 4; }

st_len_t st_utf32be_from_codepoint(const char* input, st_uc4_t codepoint) {
  st_uc4_t* p = (st_uc4_t*)input;
  *p = codepoint;

  return 4;
}

st_len_t st_utf32le_from_codepoint(const char* input, st_uc4_t codepoint) {
  assert(false); // TODO
  return 0;
}
