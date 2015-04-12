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

double st_base2number(string *src, int base) {
  assert(src->encoding == st_enc_ascii);

  size_t num = 0;
  double fnum = 0;
  size_t i;
  int mode = 0;
  char c;
  char* s = src->value;

  size_t cutoff = INT64_MAX / base;
  int cutlim = INT64_MAX % base;

  for (i = src->length; i > 0; i--) {
    c = *s++;
    /* might not work for EBCDIC */
    if (c >= '0' && c <= '9') {
      c -= '0';
    } else if (c >= 'A' && c <= 'Z') {
      c -= 'A' - 10;
    } else if (c >= 'a' && c <= 'z') {
      c -= 'a' - 10;
    } else {
      continue;
    }

    if (c >= base) {
      continue;
    }

    switch (mode) {
      case 0: /* Integer */
      if (num < cutoff || (num == cutoff && c <= cutlim)) {
        num = num * base + c;
        break;
      } else {
        fnum = (double)num;
        mode = 1;
      }
      /* fall-through */
      case 1: /* Float */
      fnum = fnum * base + c;
    }
  }

  if (mode == 1) {
    return fnum;
  }

  return num;
}


string* st_number2base(size_t value, int base) {
  static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  string* result;
  char *ptr;
  char *end;
  char buf[(sizeof(double) << 3) + 1]; // maximum size for binary

  end = ptr = buf + sizeof(buf) - 1;
  *ptr = '\0';
  // reverse buffer filling data, ptr will be char* containing data
  do {
    *--ptr = digits[value % base];
    value /= base;
  } while (ptr > buf && value);
  //?? *end = '\0';
  // size
  result = st_new(end - ptr, st_enc_ascii);
  st_copyc(&result, ptr, st_enc_ascii);
  //result->length = end - ptr;
  return result;
}

string* st_chr(st_uc4_t value, st_enc_t enc) {
  if (enc == st_enc_ascii) {
    assert(value < 256);
  }

  string* out;
  char* dst;


  switch(enc) {
    case st_enc_binary:
    case st_enc_ascii:
      out = st_new(1, enc);
      dst = out->value;

      dst[0] = value;
      dst[1] = '\0';

      out->length = 1;
      out->used = 1;
    break;

    case st_enc_utf8:
      if (value < 0x80) {
        out = st_new(1, enc);
        dst = out->value;

        dst[0] = (char)value;
        dst[1] = '\0';

        out->length = 1;
        out->used = 1;
        return out;
      }

      if (value < 0x800) {
        out = st_new(2, enc);
        dst = out->value;

        dst[0] = (value >> 6) | 0xC0;
        dst[1] = (value & 0x3F) | 0x80;
        dst[2] = '\0';

        out->length = 1;
        out->used = 2;
        return out;
      }

      if (value < 0x10000) {
        out = st_new(3, enc);
        dst = out->value;

        dst[0] = (value >> 12) | 0xE0;
        dst[1] = ((value >> 6) & 0x3F) | 0x80;
        dst[2] = (value & 0x3F) | 0x80;
        dst[3] = '\0';

        out->length = 1;
        out->used = 3;
        return out;
      }

      if (value < 0x110000) {
        out = st_new(4, enc);
        dst = out->value;

        dst[0] = (value >> 18) | 0xF0;
        dst[1] = ((value >> 12) & 0x3F) | 0x80;
        dst[2] = ((value >> 6) & 0x3F) | 0x80;
        dst[3] = (value & 0x3F) | 0x80;
        dst[4] = '\0';

        out->length = 1;
        out->used = 4;
        return out;
      }
    break;

    case st_enc_ucs4be:
      out = st_new(4, enc);
      dst = out->value;

      ((uint32_t*) out->value)[0] = value;
    break;

  }

  return out;
}


size_t st_ord(const string* str, st_len_t offset) {
  assert(str->length > offset);

  st_enc_t enc = str->encoding;
  const char* itr = str->value;
  switch(enc) {
    case st_enc_binary:
    case st_enc_ascii:
      itr +=offset;
      return (size_t) *itr;
    break;

    case st_enc_utf8: {
      while(offset--) {
        itr += string_utf8_jump_next(itr);
      }

      size_t out = (unsigned char) *itr;
      if (out <= 0x7F) {
        return out;
      }
      if (out < 0xC2) {
        return 0; // invalid?
      }

      if (out <= 0xDF) {
        return (out & 0x1F) << 6
        | (*(itr + 1) & 0x3F);
      }

      if (out <= 0xEF) {
        return (out & 0x0F) << 12
        | (*(itr + 1) & 0x3F) << 6
        | (*(itr + 2) & 0x3F);
      }

      if (out <= 0xF4) {
        return (out & 0x0F) << 18
        | (*(itr + 1) & 0x3F) << 12
        | (*(itr + 2) & 0x3F) << 6
        | (*(itr + 3) & 0x3F);
      }

      return 0;
    }
    break;
    case st_enc_ucs4be: {
      itr += offset * 4;
      size_t out = 0;

      return (size_t) *((size_t*) itr);
    }
    break;
  }

  return 0;
}
