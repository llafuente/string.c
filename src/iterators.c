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

void st_char_iterator(const string* str, st_char_itr_cb itr_cb) {
  // maximum char size is 7 bytes
  // 6 utf-8 + null
  st_enc_t enc = str->encoding;
  string* buffer = st_new(7, enc);
  buffer->length = 1;
  char* s = buffer->value;
  char* dst;

  st_len_t pos = 0;
  const char* itr = str->value;
  const char* end = itr + str->capacity - 1;

  switch(enc) {
  case st_enc_ascii:
    // \0 + end
    while (*itr && itr < end) {
      dst = s;

      st_copy_CHARS(itr, dst, 1);
      *dst = '\0';

      itr_cb(buffer, pos, str);
      ++pos;
    }
    break;
  case st_enc_utf8:
    // \0 + end
    while (*itr && itr < end) {
      dst = s;
      int jump = string_utf8_jump_next(itr);

      st_copy_CHARS(itr, dst, jump);
      *dst = '\0';

      itr_cb(buffer, pos, str);
      ++pos;
    }
    break;
  case st_enc_ucs4be:
    // \0 + end
    while (*itr && itr < end) {
      dst = s;

      st_copy_CHARS(itr, dst, 4);
      *dst = '\0';

      itr_cb(buffer, pos, str);
      ++pos;
    }
  }


  st_delete(&buffer);
}
