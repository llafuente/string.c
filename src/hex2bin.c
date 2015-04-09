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

string *st_hex2bin(string *src) {
  size_t target_length = src->length >> 1;
  char* src_val = src->value;
  string *out = st_new(target_length, string_enc_ascii);
  unsigned char *out_val = (unsigned char *)out->value;
  size_t i, j;
  unsigned char c, d;
  for (i = j = 0; i < target_length; i++) {
    c = src_val[j++];
    if (c >= '0' && c <= '9') {
      d = (c - '0') << 4;
    } else if (c >= 'a' && c <= 'f') {
      d = (c - 'a' + 10) << 4;
    } else if (c >= 'A' && c <= 'F') {
      d = (c - 'A' + 10) << 4;
    } else {
      st_delete(&out);
      return 0;
    }
    c = src_val[j++];
    if (c >= '0' && c <= '9') {
      d |= c - '0';
    } else if (c >= 'a' && c <= 'f') {
      d |= c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      d |= c - 'A' + 10;
    } else {
      st_delete(&out);
      return 0;
    }
    out_val[i] = d;
  }
  out_val[i] = '\0';
  out->length = i;
  out->used = i;
  return out;
}
