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

// ascii
void string_char_up(string* str, st_len_t pos) {
  assert(str->length > pos);

  char* val = str->value;
  char c = val[pos];

  if ((c > 96) && (c < 123)) {
    val[pos] -= 32;
  }
}

void string_char_low(string* str, st_len_t pos) {
  assert(str->length > pos);

  char* val = str->value;
  char c = val[pos];

  if ((c > 64) && (c < 97)) {
    val[pos] += 32;
  }
}

// http://pastebin.com/fuw4Uizk
void string_capitalize(string* str) { assert(str->encoding == st_enc_ascii); }

string* string_ucfirst(string* src) { return 0; }

void st_upper_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;

  // ascci do direct translation
  if (uc < 127) {
    if ((uc > 96) && (uc < 123)) {
      *character->value -= 32;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st__utf8c_to_utf32cp((st_uc_t*)character->value);
  cp = st_utf32_uppercase(cp);
  st_len_t zero_null_pos = st__utf32cp_to_utf8c(cp, (st_uc_t*)character->value);
  character->value[zero_null_pos] = '\0';
}

// TODO improve performance
string* st_upper(const string* src) { return st_char_map(src, st_upper_cb); }

string* string_lcfirst(string* src) { return 0; }

void st_lower_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;

  // ascci do direct translation
  if (uc < 127) {
    if ((uc > 64) && (uc < 97)) {
      *character->value += 32;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st__utf8c_to_utf32cp((st_uc_t*)character->value);
  cp = st_utf32_lowercase(cp);
  st_len_t zero_null_pos = st__utf32cp_to_utf8c(cp, (st_uc_t*)character->value);
  character->value[zero_null_pos] = '\0';
}

// TODO improve performance
string* st_lower(const string* src) { return st_char_map(src, st_lower_cb); }

string* string_swapcase(string* src) { return 0; }
