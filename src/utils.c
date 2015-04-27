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

void st_charmask(const char* input, st_size_t len, char* mask) {
  const char* end;

  memset(mask, 0, 256);

  for (end = input + len; input < end; ++input) {
    mask[*input] = 1;
  }
}

st_len_t st_length(const char* src, st_enc_t enc) {
  switch (enc) {
  case st_enc_ascii:
  case st_enc_binary:
    return strlen(src);
  case st_enc_utf8:
    return st_utf8_length((const char*)src, 0);
  case st_enc_utf32be:
  case st_enc_utf32le:
    return wcslen((int*)src);
  }
}

st_size_t st_capacity(const char* src, st_enc_t enc) {
  switch (enc) {
  case st_enc_ascii:
  case st_enc_binary:
  case st_enc_utf8:
    return strlen(src);
  case st_enc_utf32be:
  case st_enc_utf32le:
    return wcslen((int*)src) * 4;
  }
}

void st_get_meta(const char* src, st_enc_t enc, st_len_t* len,
                 st_size_t* bytes) {
  switch (enc) {
  case st_enc_ascii:
  case st_enc_binary:
    *len = *bytes = strlen(src);
    return;
  case st_enc_utf8:
    *len = st_utf8_length((const char*)src, bytes);
    return;
  case st_enc_utf32be:
  case st_enc_utf32le:
    *len = wcslen((int*)src);
    *bytes = *len * 4;
  }
}

void st_hexdump(const char* p, st_size_t size) {
  int n;
  for (n = 0; n < size; ++n) {
    if (n % 12 == 0) {
      printf("\n");
    }
    printf("| %2.2x ", (*p) & 0xff);
    ++p;
  }
}

void st_debug(string* str) {
  printf("st_debug @%p, length[%d] used[%d] size[%u] enc[%d]\n", str,
         str->length, str->used, str->capacity, str->encoding);

  char* p = str->value;
  size_t size = str->capacity;
  printf("\nhexadecimal\n");
  st_hexdump(p, size);

  printf("\nchar by char\n");
  p = str->value;

  int n;
  for (n = 0; n < str->length; ++n) {
    if (n < 10) {
      printf("| %d ", n);
    } else if (n < 100) {
      printf("| %d", n);
    } else {
      printf("|%d", n);
    }
  }
  printf("|\n");
  switch (str->encoding) {
  case st_enc_binary:
  case st_enc_ascii:
    for (n = 0; n < size; ++n) {
      printf("| %c ", *p ? *p : ' ');
      ++p;
    }
    break;
  case st_enc_utf8:
    for (n = 0; n < str->length; ++n) {
      string* x = st_char_at(str, n);
      printf("| %s ", x->value);
      st_delete(&x);
    }
    break;
  case st_enc_utf32be:
  case st_enc_utf32le:
    // encode wide char -> utf8
    // print
    printf("utf32 output TODO\n");
    break;
  }
  // write a null at the end, to no overflow
  // this modify the string... but it's for debug only!
  str->value[str->capacity - 1] = '\0';

  printf("\nprintf %s\n", str->value);
}

st_enc_t st_detect_encoding(char* input) {
  st_len_t len = strlen(input);

  // UTF32 4-byte BOM
  if (len >= 4) {
    st_uc4_t uc = (st_uc4_t) * ((st_uc4_t*)input);
    // BE
    if (uc == 0x0000FEFF && st_validate_encoding(input, st_enc_utf32be)) {
      return st_enc_utf32be;
    }
    // LE
    if (uc == 0xFFFE0000 && st_validate_encoding(input, st_enc_utf32le)) {
      return st_enc_utf32le;
    }
  }

  // ASCII
  if (st_validate_encoding(input, st_enc_ascii))
    return st_enc_ascii;

  // UTF8
  if (st_validate_encoding(input, st_enc_utf8))
    return st_enc_utf8;

  // unknown encoding -> binary!
  return st_enc_binary;
}

st_len_t st_char_size_safe(const char* input, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
    return 1;
  case st_enc_ascii:
    return st_ascii_char_size_safe(input);
  case st_enc_utf8:
    return st_utf8_char_size_safe(input);
  case st_enc_utf32le:
    return st_utf32le_char_size_safe(input);
  case st_enc_utf32be:
    return st_utf32be_char_size_safe(input);
  }
}

st_len_t st_char_size(const char* input, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
    return 1;
  case st_enc_ascii:
    return st_ascii_char_size(input);
  case st_enc_utf8:
    return st_utf8_char_size(input);
  case st_enc_utf32le:
    return st_utf32le_char_size(input);
  case st_enc_utf32be:
    return st_utf32be_char_size(input);
  }
}

bool st_validate_encoding(char* input, st_enc_t enc) {
  // start at the beginning
  size_t pos = 0;
  char* p = input;
  while (*p) {
    // check if the character is valid
    st_len_t len = st_char_size_safe(input, enc);
    printf("size [%d] @[%ld]\n", len, input - p);

    if (len < 0) {
      return false;
    }
    // move to the next character
    if (enc == st_enc_utf8) {
      // check surrogates

      while (++p && --len) {
        if (!ST_UTF8_IS_TRAIL(*p)) {
          return false;
        }
      }
    } else {
      input += len;
    }
    printf("next char [%c]\n", *input);
  }
  // if we didn't fail yet, return success
  return true;
}

st_uc4_t st_codepoint(const char* str, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
    return (st_uc4_t)*str;
  case st_enc_ascii:
    return st_ascii_codepoint(str);
  case st_enc_utf8:
    return st_utf8_codepoint(str);
  case st_enc_utf32le:
    return st_utf32le_codepoint(str);
  case st_enc_utf32be:
    return st_utf32be_codepoint(str);
  }

  return 0;
}

st_len_t st_from_codepoint(char* out, st_uc4_t codepoint, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
    assert(false); // TODO how to handle this? as ascii? as 4 bytes?
  case st_enc_ascii:
    return st_ascii_from_codepoint(out, codepoint);
  case st_enc_utf8:
    return st_utf8_from_codepoint(out, codepoint);
  case st_enc_utf32le:
    return st_utf32le_from_codepoint(out, codepoint);
  case st_enc_utf32be:
    return st_utf32be_from_codepoint(out, codepoint);
  }

  return 0;
}
