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

double st_base2number(string* src, int base) {
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
  char* ptr;
  char* end;
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
  // result->length = end - ptr;
  return result;
}

string* st_chr(st_uc4_t value, st_enc_t enc) {
  if (enc == st_enc_ascii) {
    assert(value < 256);
  }

  string* out;
  char* dst;

  switch (enc) {
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

  case st_enc_utf32le:
  case st_enc_utf32be:
    out = st_new(4, enc);
    dst = out->value;

    ((uint32_t*)out->value)[0] = value;
    break;
  }

  return out;
}

st_uc4_t st_ord(const string* str, st_len_t offset) {
  assert(str->length > offset);

  st_enc_t enc = str->encoding;
  const char* itr = str->value;
  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii:
    itr += offset;
    return (st_uc4_t)*itr;
    break;

  case st_enc_utf8: {
    while (offset--) {
      itr += st_utf8_char_size(itr);
    }

    st_uc_t out = (st_uc_t)*itr;
    if (out <= 0x7F) {
      return out;
    }
    if (out < 0xC2) {
      return 0; // invalid?
    }

    if (out <= 0xDF) {
      return (out & 0x1F) << 6 | (*(itr + 1) & 0x3F);
    }

    if (out <= 0xEF) {
      return (out & 0x0F) << 12 | (*(itr + 1) & 0x3F) << 6 |
             (*(itr + 2) & 0x3F);
    }

    if (out <= 0xF4) {
      return (out & 0x0F) << 18 | (*(itr + 1) & 0x3F) << 12 |
             (*(itr + 2) & 0x3F) << 6 | (*(itr + 3) & 0x3F);
    }

    return 0;
  } break;
  case st_enc_utf32be:
    itr += offset * 4;
    return st_utf32be_codepoint(itr);
  case st_enc_utf32le:
    itr += offset * 4;
    return st_utf32le_codepoint(itr);
  }

  return 0;
}

string* st_unescape(const string* str) {
  st_uc_t ch;
  st_uc_t oval;
  int i;

#define ISOCTAL(c) ((c >= (st_uc_t)'0') && (c <= (st_uc_t)'7'))

  string* out = st_new(str->capacity, str->encoding);
  const char* data = str->value;

  while ((ch = *(st_uc_t*)(data++)) != 0) {
    if (ch == '\\') {
      if ((ch = *((st_uc_t*)data++)) == 0)
        break;
      switch (ch) {
      case '\\': /* \\ -> \ */
        ch = '\\';
        break;
      case 'a': /* \a -> audible bell */
        ch = '\a';
        break;
      case 'b': /* \b -> backspace */
        ch = '\b';
        break;
      case 'f': /* \f -> formfeed */
        ch = '\f';
        break;
      case 'n': /* \n -> newline */
        ch = '\n';
        break;
      case 'r': /* \r -> carriagereturn */
        ch = '\r';
        break;
      case 't': /* \t -> horizontal tab */
        ch = '\t';
        break;
      case 'v': /* \v -> vertical tab */
        ch = '\v';
        break;
      case '0': /* \nnn -> ASCII value */
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
        for (oval = ch - '0', i = 0;
             i < 2 && (ch = *((st_uc_t*)data)) != 0 && ISOCTAL(ch);
             i++, data++) {
          oval = (oval << 3) | (ch - '0');
        }
        ch = oval;
        break;
      default: /* \any -> any */
        break;
      }
    }

    st_append_char(&out, ch);
  }

  st__zeronull(out->value, out->used, out->encoding);

  return out;
}

string* st_escape(const string* str) {
  st_uc_t ch;

  const char* data = str->value;
  string* out = st_new(str->capacity, str->encoding);
  string* buffer = st_new(5, str->encoding);

#define ISDIGIT(c) ((c >= (st_uc_t)'0') && (c <= (st_uc_t)'9'))
  st_size_t len = str->used;
  while (len-- > 0) {
    ch = *(st_uc_t*)(data++);
    if (ch < 127) {
      if (isprint(ch)) {
        if (ch == '\\') {
          st_append_char(&out, ch);
        }
        st_append_char(&out, ch);
        continue;
      } else if (ch == '\a') { /* \a -> audible bell */
        ch = (st_uc_t)'a';
      } else if (ch == '\b') { /* \b -> backspace */
        ch = (st_uc_t)'b';
      } else if (ch == '\f') { /* \f -> formfeed */
        ch = (st_uc_t)'f';
      } else if (ch == '\n') { /* \n -> newline */
        ch = (st_uc_t)'n';
      } else if (ch == '\r') { /* \r -> carriagereturn */
        ch = (st_uc_t)'r';
      } else if (ch == '\t') { /* \t -> horizontal tab */
        ch = (st_uc_t)'t';
      } else if (ch == '\v') { /* \v -> vertical tab */
        ch = (st_uc_t)'v';
      }
      st_append_char(&out, '\\');
      st_append_char(&out, ch);
      continue;
    }
    if (ISDIGIT(ch)) {
      sprintf(buffer->value, "\\%03d", ch);
      st_append(&out, buffer);
    } else {
      sprintf(buffer->value, "\\%d", ch);
      st_append(&out, buffer);
    }
  }

  st__zeronull(out->value, out->used, out->encoding);
  st_delete(&buffer);

  return out;
}
