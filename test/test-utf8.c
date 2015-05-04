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

#include "stringc.h"
#include "fixtures.h"
#include "tasks.h"

TASK_IMPL(utf8) {
  st_size_t cap;
  ASSERT(st_utf8_length(T_STR_01, &cap) == 12, "length check");
  ASSERT(strlen(T_STR_01) == cap, "capacity check");
  ASSERT(st_utf8_length(T_STR_02, &cap) == 33, "length check");
  ASSERT(strlen(T_STR_02) == cap, "capacity check");
  ASSERT(st_utf8_length(T_STR_03, &cap) == 6, "length check");
  ASSERT(strlen(T_STR_03) == cap, "capacity check");
  ASSERT(st_utf8_length(T_STR_03_REP2, &cap) == 12, "length check");
  ASSERT(strlen(T_STR_03_REP2) == cap, "capacity check");
  ASSERT(st_utf8_length(T_STR_03_REP3, &cap) == 18, "length check");
  ASSERT(strlen(T_STR_03_REP3) == cap, "capacity check");
  ASSERT(st_utf8_length(T_STR_03_REP4, &cap) == 24, "length check");
  ASSERT(strlen(T_STR_03_REP4) == cap, "capacity check");

  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_1,
                         strlen(T_STR_UTF8_1)) == 0,
         "T_STR_UTF8_1 is valid");
  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_2,
                         strlen(T_STR_UTF8_2)) == 0,
         "T_STR_UTF8_2 is valid");
  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_3,
                         strlen(T_STR_UTF8_3)) == 0,
         "T_STR_UTF8_3 is valid");
  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_4,
                         strlen(T_STR_UTF8_4)) == 0,
         "T_STR_UTF8_4 is valid");

  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_5,
                         strlen(T_STR_UTF8_5)) == T_STR_UTF8_5 + 1,
         "T_STR_UTF8_5 is invalid");
  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_6,
                         strlen(T_STR_UTF8_6)) == T_STR_UTF8_6 + 5,
         "T_STR_UTF8_6 is invalid");

  ASSERT(st_utf8_invalid((const unsigned char*)T_STR_UTF8_7,
                         strlen(T_STR_UTF8_7)) == 0,
         "T_STR_UTF8_7 is valid");

  ASSERT(st_validate_encoding(T_STR_UTF8_5, st_enc_utf8) == false,
         "validate T_STR_UTF8_5");
  ASSERT(st_validate_encoding(T_STR_UTF8_7, st_enc_utf8) == true,
         "validate T_STR_UTF8_7");
  ASSERT(st_validate_encoding(T_STR_UTF8_2, st_enc_utf8) == true,
         "validate T_STR_UTF8_2");

  ASSERT(st_utf8_char_eq("☃", "☃") == true, "☃ == ☃?");
  ASSERT(st_utf8_char_eq("ñ", "ñ") == true, "ñ == ñ?");
  ASSERT(st_utf8_char_eq("☃", "a") == false, "☃ == a?");
  ASSERT(st_utf8_char_eq("A", "a") == false, "A == a?");
  ASSERT(st_utf8_char_eq("ñ", "☃") == false, "ñ == ☃?");

  ASSERT(st_utf8_valid_codepoint(0x0000662F) == true,
         "UTF8 cp 是 is valid"); // 是
  ASSERT(st_utf8_valid_codepoint(0x0002A6A5) == true,
         "UTF8 cp 龍 is valid"); // 龍×4
  ASSERT(st_utf8_valid_codepoint(0xFFFFFFFF) == false,
         "UTF8 cp 0xFFFFFFFF is invalid"); //
  // TODO ASSERT(st_utf8_valid_codepoint(0x0000C080) == false, "UTF8 cp 0xC080
  // is invalid"); // RFC 3629
  // TODO ASSERT(st_utf8_valid_codepoint(0x0000C0AE) == false, "UTF8 cp 0xC0AE
  // is invalid"); // RFC 3629

  ASSERT(st_utf8_char_eq("龍", "龍") == true, "龍 == 龍");
  ASSERT(st_utf8_char_eq("龍", "是") == false, "龍 != 是");
  ASSERT(st_utf8_char_eq("是", "是") == true, "是 == 是");
  ASSERT(st_utf8_char_eq("是", "龍") == false, "是 != 是");
  ASSERT(st_utf8_char_eq("☃", "☃") == true, "☃ == ☃");
  ASSERT(st_utf8_char_eq("☃", "ñ") == false, "☃ != ñ");
  ASSERT(st_utf8_char_eq("☃", "是") == false, "☃ != 是");
  ASSERT(st_utf8_char_eq("ñ", "ñ") == true, "ñ == ñ");
  ASSERT(st_utf8_char_eq("ñ", "☃") == false, "ñ != ☃");

  ASSERT(st_utf8_char_size_safe("是") == 3, "length(是) = 3");
  ASSERT(st_utf8_char_size_safe("龍") == 3, "length(龍) = 3");
  // http://www.i18nguy.com/unicode/supplementary-test.html
  ASSERT(st_utf8_char_size_safe("𠜎") == 4, "length(𠜎) = 4");

  return 0;
}
