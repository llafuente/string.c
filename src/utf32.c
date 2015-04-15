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

st_uc4_t st_utf32be_codepoint(const st_uc_t* input) {
  return ((input[0]) << 24) + ((input[1]) << 16) + ((input[2]) << 8) +
         (input[3]);
}

st_uc4_t st_utf32le_codepoint(const st_uc_t* input) {
  return ((input[3]) << 24) + ((input[2]) << 16) + ((input[1]) << 8) +
         (input[0]);
}

st_len_t st_utf32_char_size(st_uc_t lead_chr) { return 4; }
