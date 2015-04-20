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

void st__repeat(char* dst, const char* src, size_t src_len, size_t times) {
  // Heavy optimization for situations where src string is 1 byte long
  if (src_len == 1) {
    memset(dst, *(src), times);
    return;
  }

  st_len_t result_len = src_len * times;

  char* s, *e, *ee;
  // TODO review: ptrdiff_t l=0;
  size_t l = 0;
  memcpy(dst, src, src_len);
  s = dst;
  e = dst + src_len;
  ee = dst + result_len;
  while (e < ee) {
    l = (e - s) < (ee - e) ? (e - s) : (ee - e);
    memmove(e, s, l);
    e += l;
  }
}

void st__zeronull(char* str, size_t bytepos, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii:
  case st_enc_utf8:
    str[bytepos] = '\0';
    break;
  case st_enc_utf32le:
  case st_enc_utf32be: // null is also, 4 bytes!
                       // test L'\0' and cast st_uc4_t/wchar_t
    str[bytepos] = '\0';
    str[bytepos + 1] = '\0';
    str[bytepos + 2] = '\0';
    str[bytepos + 3] = '\0';
  }
}

size_t st__zeronull_size(st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii:
  case st_enc_utf8:
    return 1;
  case st_enc_utf32le:
  case st_enc_utf32be:
    return 4;
  }
}
