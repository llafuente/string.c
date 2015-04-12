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

const uint8_t st_bom[] = {0xef, 0xbb, 0xbf};

char* st__memchr(const char *s, st_uc_t c, size_t n) {
  if (n != 0) {
    const st_uc_t* p = (st_uc_t*) s;

    do {
      if (*p++ == c) {
        return ((char *)(p - 1));
      }
    } while (--n != 0);
  }

  return 0;
}


char* st__get_char_offset(string* str, st_len_t offset) {
  if (!offset) {
    return str->value;
  }

  assert(offset < str->length);

  if (offset < 0) {
    assert(-offset < str->length);
    offset = str->length + offset;
  }

  char* p = str->value;

  ST_ADVANCE(p, offset, str->encoding);

  return p;
}


void st__calc_range(st_len_t str_length, st_len_t* offset, st_len_t* offset_length) {
  printf("offset_length %ld\n", *offset_length);
  printf("offset %ld\n", *offset);
  printf("str_length %ld\n", str_length);

  st_len_t off = *offset;
  st_len_t off_len = *offset_length;

  if (off < 0) {
    assert(-off < str_length);
    off = str_length + off;
    *offset = off;
  }

  if (off_len == 0) {
    *offset_length = str_length - *offset;
  } else if (off_len < 0) {
    *offset_length = (str_length + off_len) - *offset;
  }

  printf("offset_length %ld\n", *offset_length);
  printf("offset %ld\n", *offset);
  printf("str_length %ld\n", str_length);

  // overflow?
  assert(*offset + *offset_length <= str_length);
}


void st__get_char_range(string* str, st_len_t offset, st_len_t len, char** start, char** end) {
  char* s =  str->value; // offset == 0
  char* e;
  st_enc_t enc = str->encoding;

  if (offset < 0) {
    assert(-offset < str->length);

    offset = str->length + offset;
    ST_ADVANCE(s, offset, enc);
  } else if (offset > 0) {
    assert(offset < str->length);

    ST_ADVANCE(s, offset, enc);
  }

  if (len == 0) {
    len = str->length - offset;
  } else if (len < 0) {
    len = (str->length + len) - offset;
  }

  // overflow?
  assert(offset + len <= str->length);

  e = s;

  ST_ADVANCE(e, len, enc);

  *start = s;
  *end = e;
}
