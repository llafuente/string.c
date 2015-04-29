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

// http://www.unicode.org/reports/tr10/
// http://www.unicode.org/Public/UCA/latest/allkeys.txt

#include "stringc.h"

#define STRING_IS_LOW_ASCII(c) (c > 96) && (c < 123)
#define STRING_LOW_ASCCI(c) c -= 32;

#define STRING_IS_UP_ASCII(c) (c > 64) && (c < 97)
#define STRING_UP_ASCCI(c) c -= 32;

void st_upper_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;
  st_enc_t enc = src->encoding;

  // ascci? do direct translation
  if ((enc == st_enc_ascii || enc == st_enc_utf8) && uc < 127) {
    if ((uc > 96) && (uc < 123)) {
      *character->value -= 32;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st_utf8_codepoint(character->value);
  cp = st_utf32_uppercase(cp);
  st_len_t zero_null_pos = st_utf8_from_codepoint(character->value, cp);
  st__zeronull(character->value, zero_null_pos, enc);
}

// TODO improve performance
string* st_upper(const string* src) { return st_char_map(src, st_upper_cb); }

void st_lower_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;
  st_enc_t enc = src->encoding;

  // ascci? do direct translation
  if ((enc == st_enc_ascii || enc == st_enc_utf8) && uc < 127) {
    if ((uc > 64) && (uc < 97)) {
      *character->value += 32;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st_utf8_codepoint(character->value);
  cp = st_utf32_lowercase(cp);
  st_len_t zero_null_pos = st_utf8_from_codepoint(character->value, cp);
  st__zeronull(character->value, zero_null_pos, enc);
}

// TODO improve performance
string* st_lower(const string* src) { return st_char_map(src, st_lower_cb); }

string* st_capitalize(const string* str) {
  const char* itr = str->value;
  const char* end = itr + str->used;
  st_enc_t enc = str->encoding;

  string* out = st_new_max(str->used, enc);
  char* oval = out->value;

  // fist UP
  st_len_t jump = st_char_size(itr, enc);
  st_uc4_t cp = st_codepoint(itr, enc);
  st_uc4_t cp2 = st_utf32_uppercase(cp);
  st_len_t cp_size;

  if (cp != cp2) {
    // printf("change 1\n");
    cp_size = st_from_codepoint(oval, cp2, enc);
    oval += cp_size;
  } else {
    // printf("copy 1\n");
    memcpy(oval, itr, jump);
    oval += jump;
  }

  itr += jump;

  // then down
  while (itr < end) {
    // printf("-- itr [%p] oval [%p]\n", itr, oval);

    jump = st_char_size(itr, enc);
    cp = st_codepoint(itr, enc);
    cp2 = st_utf32_lowercase(cp);

    if (cp != cp2) {
      // printf("change\n");
      cp_size = st_from_codepoint(oval, cp2, enc);
      oval += cp_size;
    } else {
      // printf("copy\n");
      memcpy(oval, itr, jump);
      oval += jump;
    }

    itr += jump;
  }

  out->used = oval - out->value;
  st__zeronull(oval, 0, enc);
  out->length = str->length;

  return out;
}

void st__char_upper(char* str, char* buffer, st_enc_t enc) {
  st_uc4_t cp = st_codepoint(str, enc);
  cp = st_utf32_uppercase(cp);
  st_len_t cp_size = st_from_codepoint(buffer, cp, enc);

  st__zeronull(buffer, cp_size, enc);
}

void st__char_lower(char* str, char* buffer, st_enc_t enc) {
  st_uc4_t cp = st_codepoint(str, enc);
  cp = st_utf32_lowercase(cp);
  st_len_t cp_size = st_from_codepoint(buffer, cp, enc);

  st__zeronull(buffer, cp_size, enc);
}

string* st_ucfirst(const string* str) {
  const char* itr = str->value;
  st_enc_t enc = str->encoding;

  // fist UP
  st_len_t jump = st_char_size(itr, enc);
  st_uc4_t cp = st_codepoint(itr, enc);
  st_uc4_t cp2 = st_utf32_uppercase(cp);
  st_len_t cp_size;

  if (cp != cp2) {
    string* out = st_new_max(str->used, enc);
    char* oval = out->value;

    cp_size = st_from_codepoint(oval, cp2, enc);

    memcpy(oval + cp_size, itr + jump,
           str->used - jump + st__zeronull_size(enc));

    out->length = str->length;
    out->used = str->used - jump + cp_size;

    return out;
  }

  return st_clone(str);
}

string* string_swapcase(string* src) { return 0; }

string* st_lcfirst(string* str) {
  const char* itr = str->value;
  st_enc_t enc = str->encoding;

  // fist UP
  st_len_t jump = st_char_size(itr, enc);
  st_uc4_t cp = st_codepoint(itr, enc);
  st_uc4_t cp2 = st_utf32_lowercase(cp);
  st_len_t cp_size;

  if (cp != cp2) {
    string* out = st_new_max(str->used, enc);
    char* oval = out->value;

    cp_size = st_from_codepoint(oval, cp2, enc);

    memcpy(oval + cp_size, itr + jump,
           str->used - jump + st__zeronull_size(enc));

    out->length = str->length;
    out->used = str->used - jump + cp_size;

    return out;
  }

  return st_clone(str);
}
