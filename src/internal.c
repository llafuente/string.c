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

const st_uc_t st_bom[] = {0xef, 0xbb, 0xbf};

char* st__memchr(const char* s, st_uc_t c, size_t n) {
  if (n != 0) {
    const st_uc_t* p = (st_uc_t*)s;

    do {
      if (*p++ == c) {
        return ((char*)(p - 1));
      }
    } while (--n != 0);
  }

  return 0;
}

char* st__mempbrk(const char* s, const char* accept) {

  while (*s != '\0') {
    const char* a = accept;

    while (*a != '\0') {
      if (*a++ == *s) {
        return (char*)s;
      }
    }

    ++s;
  }

  return 0;
}

char* st__get_char_offset(const string* str, st_len_t offset) {
  if (!offset) {
    return (char*)str->value;
  }

  assert(offset < str->length);

  if (offset < 0) {
    assert(-offset < str->length);
    offset = str->length + offset;
  }

  char* p = (char*)str->value;

  ST_ADVANCE(p, offset, str->encoding);

  return p;
}

void st__calc_range(st_len_t str_length, st_len_t* offset, st_len_t* length) {
  st_len_t off = *offset;
  st_len_t off_len = *length;

  if (off < 0) {
    assert(-off < str_length);
    off = str_length + off;
    *offset = off;
  }

  if (off_len == 0) {
    *length = str_length - *offset;
  } else if (off_len < 0) {
    *length = (str_length + off_len);
  } else {
    *length = *offset + off_len;
  }

  // overflow?
  assert(*length <= str_length);
}

void st__get_char_range(const string* str, st_len_t offset, st_len_t length,
                        char** start, char** end) {
  char* s = (char*)str->value; // start == 0
  char* e;
  st_enc_t enc = str->encoding;

  assert(offset >= 0);
  assert(length >= 0);

  if (offset) {
    ST_ADVANCE(s, offset, enc);
  }

  e = s;
  if (length) {
    ST_ADVANCE(e, length, enc);
  }

  *start = s;
  *end = e;
}

st_uc4_t st__utf8c_to_utf32cp(st_uc_t* utf8) {
  st_uc_t c = *utf8;

  if ((c < (st_uc_t)'\200')) {
    return c;
  }

  if ((c >= (st_uc_t)'\370')) {
    return 0;
  }

  if ((c >= (st_uc_t)'\360')) {
    return ((((262144 * (c % 8)) + (4096 * (utf8[1] % 64))) +
             (64 * (utf8[2] % 64))) +
            (utf8[3] % 64));
  }

  if ((c >= (st_uc_t)'\340')) {
    return (((4096 * (c % 16)) + (64 * (utf8[1] % 64))) + (utf8[2] % 64));
  }

  if ((c >= (st_uc_t)'\300')) {
    return ((64 * (c % 32)) + (utf8[1] % 64));
  }

  return 0;
}

// utf32 code point to utf8 chars
st_len_t st__utf32cp_to_utf8c(st_uc4_t utf32, st_uc_t* utf8) {
  if (utf32 < 128) {
    utf8[0] = utf32;
    return 1;
  }

  if (utf32 < 2048) {
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (192 + (utf32 % 32));
    return 2;
  }

  if (utf32 < 65536) {
    utf8[2] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (224 + utf32);
    return 3;
  }

  if (utf32 < 1048576) {
    utf8[3] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[2] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (240 + utf32);
    return 4;
  }
  return 0; // err
}
