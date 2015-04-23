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

// 1 left
// 2 right
// 3 center
string* st_justify(const string* src, st_len_t width, const string* padstr,
                   char mode) {
  // cache
  st_len_t len = src->length;

  if (width < 0 || len >= width) {
    return st_clone(src);
  }

  st_enc_t enc = src->encoding;
  size_t src_used = src->used;

  st_len_t pad_len;
  char* pad_val;

  if (!padstr) { // 0
    pad_val = " ";
    pad_len = 1;
  } else {
    assert(enc == padstr->encoding);
    pad_len = padstr->used;
    pad_val = (char*)padstr->value;
  }

  string* out = st_new_max(width, enc);

  char* p = out->value;

  st_len_t n = width - len;
  st_len_t l_times = (mode == 1) ? 0 : ((mode == 2) ? n : n / 2);
  st_len_t r_times = n - l_times;

  // printf("n = %ld\n", n);
  // printf("l_times = %ld\n", l_times);
  // printf("r_times = %ld\n", r_times);

  st_len_t offset = 0;
  if (l_times) {
    st__repeat(p, pad_val, pad_len, l_times);
    offset = pad_len * l_times;
  }

  p[offset] = '\0';
  // printf("[%ld] %s\n", offset, p);

  memcpy(p + offset, src->value, src_used);
  offset += src_used;

  p[offset] = '\0';
  // printf("[%ld] %s\n", offset, p);

  if (r_times) {
    st__repeat(p + offset, pad_val, pad_len, r_times);
    offset += pad_len * r_times;
  }

  p[offset] = '\0';
  // printf("[%ld] %s\n", offset, p);

  out->used = offset;
  out->length = r_times + l_times + len;

  return out;
}

string* st_center(const string* src, size_t width, const string* padstr) {
  return st_justify(src, width, padstr, 3);
}

string* st_right(const string* src, size_t width, const string* padstr) {
  return st_justify(src, width, padstr, 2);
}

string* st_left(const string* src, size_t width, const string* padstr) {
  return st_justify(src, width, padstr, 1);
}
