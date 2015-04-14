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

void st_charmask(st_uc_t* input, size_t len, char* mask) {
  st_uc_t* end;

  memset(mask, 0, 256);

  for (end = input + len; input < end; ++input) {
    mask[*input] = 1;
  }
}

st_len_t st_length(const char* src, st_enc_t enc) {
  if (enc == st_enc_ascii) {
    return strlen(src);
  } else if (enc == st_enc_utf8) {
    size_t used;
    return st_utf8_length((const char*)src, &used);
  }

  return -1;
}

size_t st_capacity(const char* src, st_enc_t enc) { return strlen(src); }

void st_get_meta(const char* src, st_enc_t enc, st_len_t* len,
                 st_len_t* bytes) {
  switch (enc) {
  case st_enc_ascii:
  case st_enc_binary:
    *len = *bytes = strlen(src);
    return;
  case st_enc_utf8:
    *len = st_utf8_length((const char*)src, bytes);
    return;
  case st_enc_ucs4be:
    *bytes = strlen(src);
    *len = *bytes * 0.25;
  }
}

void st_zeronull(string* str) { str->value[str->used] = '\0'; }

void st_debug(string* str) {
  printf("st_debug @%p, length[%zu] used[%zu] size[%zu] enc[%d]\n", str,
         str->length, str->used, str->capacity, str->encoding);

  char* p = str->value;
  size_t size = str->capacity;
  printf("\nhexadecimal\n");
  int n;
  for (n = 0; n < size; ++n) {
    printf("| %2.2x ", (*p) & 0xff);
    ++p;
  }
  printf("\nchar by char\n");
  p = str->value;

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
  case st_enc_ucs4be:
    // encode wide char -> utf8
    // print
    break;
  }
  printf("\nprintf %s\n", str->value);
}
