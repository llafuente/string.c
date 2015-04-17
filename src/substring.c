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

// unsafe!
// same encoding
st_len_t st_copy_usub(string* out, st_len_t initial_byte, const string* src,
                      st_len_t start, size_t todo) {
  char* dst = out->value + initial_byte;
  const char* itr = src->value;

  assert(out->encoding == src->encoding);
  st_enc_t enc = out->encoding;

  st_len_t done = 0;

  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii: {
    // \0 + end

    itr += start;
    const char* end = itr + todo;

    while (*itr && itr < end) {
      *dst = *itr;

      ++itr;
      ++dst;
      ++done;
    }
    *dst = '\0';
  } break;
  case st_enc_utf8: {
    // \0 + end
    while (start--) {
      itr += st_utf8_char_size(itr);
    }

    while (*itr && todo--) {
      int jump = st_utf8_char_size(itr);
      done += jump;

      ST_CHAR_CP(dst, itr, jump, false);

      dst += jump;
      itr += jump;
    }
    *dst = '\0';
  } break;
  case st_enc_utf32le:
  case st_enc_utf32be: {
    // \0 + end
    itr += start * 4;
    const char* end = itr + (todo * 4);

    while (*itr && itr < end) {
      ST_CHAR_CP(dst, itr, 4, false);
      dst += 4;
      itr += 4;
      done += 4;
    }
    *dst = '\0';
  } break;
  }

  return done;
}

string* st_sub(const string* str, int start, int end) {
  assert(end < str->length);
  assert(end > -str->length);

  size_t len = 0;
  string* out = 0;
  size_t out_byte_ptr = 0;

  if (start < 0) {
    assert(-start < str->length);
    out = st_new(str->capacity * 2, str->encoding);

    out_byte_ptr = st_copy_usub(out, 0, str, str->length + start, -start);

    out->length = -start;
    out->used = out_byte_ptr;
    start = 0;
  } else {
    out = st_new(str->capacity, str->encoding);
  }

  if (end == 0) { // end of string!
    len = str->length - start;
  } else {
    len = end - start;
  }

  out_byte_ptr = st_copy_usub(out, out_byte_ptr, str, start, len);
  out->used += out_byte_ptr;
  out->length += len;

  return out;
}
