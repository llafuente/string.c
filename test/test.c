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

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <fcntl.h>

//
// working group
//

#define T_STR_ASCII                                                            \
  " !\"#$%&'()*+,-./"                                                          \
  "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"                         \
  "abcdefghijklmnopqrstuvwxyz{|}~"
#define T_STR_ASCII_UTF32                                                      \
  L" !\"#$%&'()*+,-./"                                                         \
  L"0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"                        \
  L"abcdefghijklmnopqrstuvwxyz{|}~"
#define T_STR_01 "hello world!"
#define T_STR_02 "hello my friend i'm even larger!!"
#define T_STR_03 "niño ☃"
#define T_STR_03_PLUS_ONE "niño ☃"
#define T_STR_03_REP2 "niño ☃niño ☃"
#define T_STR_03_REP3 "niño ☃niño ☃niño ☃"
#define T_STR_03_REP4 "niño ☃niño ☃niño ☃niño ☃"
#define T_STR_5BIN "101"
#define T_STR_CMP1 "b"
#define T_STR_CMP2 "a"
#define T_STR_CMP3 "c"

#define T_STR_UTF8_1 "Maïté"
#define T_STR_UTF8_2 "Léa"
#define T_STR_UTF8_3 "Pauline"
#define T_STR_UTF8_4 "Élise"
#define T_STR_UTF8_5 "\xc9\x6c\x69\x73\x65"
#define T_STR_UTF8_6 "Elis\xc9"
#define T_STR_UTF8_7 "Привет"

#define T_STR_CAP_1 "word"
#define T_STR_CAP_2 "word up"
#define T_STR_CAP_3 "WORD UP"
#define T_STR_CAP_T1 "Word"
#define T_STR_CAP_T2 "Word up"
#define T_STR_CAP_T3 "Word up"

//
// assert macro + backtrace
//

void print_trace(void) {
  void* array[10];
  size_t size;
  char** strings;
  size_t i;

  size = backtrace(array, 10);
  strings = backtrace_symbols(array, size);

  printf("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++) {
    printf("%s\n", strings[i]);
  }
  free(strings);
}

#define ASSERT(comparison, name)                                               \
  if (comparison) {                                                            \
    printf("\x1B[32mtest PASS\x1B[39m: %-32s [@%d]\n", name, __LINE__);        \
  } else {                                                                     \
    printf("\x1B[31mtest FAIL\x1B[39m: %-32s [@%d]\n", name, __LINE__);        \
    print_trace();                                                             \
    exit(1);                                                                   \
  }

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define PASTE2(a, b) a##b
#define PASTE(a, b) PASTE2(a, b)

// printf("@%s:%s (%s)\n", __FILE__, STRINGIFY(__LINE__), __func__);
// all these castings are necesary for gcc
#define CHK_ALL(src, dst, enc)                                                 \
  if (enc == st_enc_utf32be || enc == st_enc_utf32le) {                        \
    printf("# CHECK %s = utf32 L[%d]U[%d]C[%u]\n", STRINGIFY(x), src->length,  \
           src->used, src->capacity);                                          \
    printf("# CHECK %s = utf32 L[%d]C[%u]\n", STRINGIFY(y),                    \
           st_length((const char*)dst, enc),                                   \
           st_capacity((const char*)dst, enc));                                \
    ASSERT(0 == wcscmp((const wchar_t*)src->value, (const wchar_t*)dst),       \
           "value");                                                           \
  } else {                                                                     \
    printf("# CHECK %s = '%s' L[%d]U[%d]C[%u]\n", STRINGIFY(x), src->value,    \
           src->length, src->used, src->capacity);                             \
    printf("# CHECK %s = '%s' L[%d]C[%u]\n", STRINGIFY(y), (char*)dst,         \
           st_length((const char*)dst, enc),                                   \
           st_capacity((const char*)dst, enc));                                \
    ASSERT(0 == strcmp((const char*)src->value, (const char*)dst), "value");   \
  }                                                                            \
  ASSERT(src->length == st_length((const char*)dst, enc), "length");           \
  ASSERT(src->used == st_capacity((const char*)dst, enc), "used");             \
  ASSERT(src->capacity >= src->used, "capacity");

#define RUN_TEST(test)                                                         \
  printf("\n\n################\n");                                            \
  printf("## %s\n", #test);                                                    \
  printf("################\n");                                                \
  test_##test();

extern void test_alloc();
extern void test_case();
extern void test_repeat();
extern void test_itr_chars();
extern void test_utf8_invalid();
extern void test_utf8_lenc();
extern void test_trim();
extern void test_compare();
extern void test_hexbinhex();
extern void test_from();
extern void test_append();
extern void test_sub();
extern void test_shuffle();
extern void test_chr();
extern void test_search();
extern void test_ascii();
extern void test_utf8();
extern void test_encoding();
extern void test_internal();
extern void test_utils();
extern void test_justify();

int main(int argc, const char* argv[]) {
  /*
  string* s = st_newc("chiwaka", st_enc_ascii);
  char* d = st_dump(s);
  printf("%s", d);
  st_delete(&s);
  free(d);

  return 1;
  */

  printf("    ########################\n");
  printf("    ## string.c unit test ##\n");
  printf("    ########################\n");

  RUN_TEST(alloc);
  RUN_TEST(case);
  RUN_TEST(repeat);
  RUN_TEST(utf8_lenc);
  RUN_TEST(utf8_invalid);
  RUN_TEST(itr_chars);
  RUN_TEST(trim);
  RUN_TEST(compare);
  RUN_TEST(hexbinhex);
  RUN_TEST(from);
  RUN_TEST(append);
  RUN_TEST(sub);
  RUN_TEST(shuffle);
  RUN_TEST(chr);
  RUN_TEST(search);
  RUN_TEST(ascii);
  RUN_TEST(utf8);
  RUN_TEST(encoding);
  RUN_TEST(internal);
  RUN_TEST(utils);
  RUN_TEST(justify);

  st_cleanup();
  printf("OK\n");

  return 0;
}

void test_alloc() {
  //
  // memory
  //
  string* s = st_new(2, st_enc_ascii);
  string* s2;

  ASSERT(s->used == 0, "string used");
  ASSERT(s->length == 0, "string length");
  ASSERT(s->capacity == 3, "string capacity"); // 3 because is null-terminated
  ASSERT(s->value[0] == '\0', "string value");

  // ASSERT(is_utf8(T_STR_02) == 0);
  // ASSERT(is_utf8(T_STR_03_REP4) == 0);

  // st_resize(&s, 50);
  st_copyc(&s, T_STR_01, st_enc_ascii);
  ASSERT(s->capacity == 13, "string T_STR_01 capacity");
  CHK_ALL(s, T_STR_01, st_enc_ascii);

  st_copyc(&s, T_STR_02, st_enc_ascii);
  CHK_ALL(s, T_STR_02, st_enc_ascii);

  st_copyc(&s, T_STR_03, st_enc_utf8);
  CHK_ALL(s, T_STR_03, st_enc_utf8);
  st_delete(&s);

  s = st_newc(T_STR_03, st_enc_utf8);
  CHK_ALL(s, T_STR_03, st_enc_utf8);

  s2 = st_clone(s);
  CHK_ALL(s2, T_STR_03, st_enc_utf8);
  ASSERT(s->capacity == s2->capacity, "s & s2 capacity must be the same");
  st_delete(&s2);

  s2 = st_rclone(s, 128);
  CHK_ALL(s2, T_STR_03, st_enc_utf8);
  ASSERT(s2->capacity == 129, "s capacity 129");
  st_delete(&s2);

  s2 = st_new(1, st_enc_utf8);
  st_copy(&s2, s);
  CHK_ALL(s2, T_STR_03, st_enc_utf8);

  st_clear(s2);
  CHK_ALL(s2, "", st_enc_utf8);

  st_delete(&s2);

  st_delete(&s);
}

void test_case() {
  string* s = st_newc("abc", st_enc_utf8);
  string* up = st_upper(s);
  CHK_ALL(up, "ABC", st_enc_utf8);

  st_delete(&s);
  st_delete(&up);

  s = st_newc("áéíóú", st_enc_utf8);

  up = st_ucfirst(s);
  CHK_ALL(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  up = st_upper(s);
  CHK_ALL(up, "ÁÉÍÓÚ", st_enc_utf8);
  st_delete(&up);

  up = st_capitalize(s);
  CHK_ALL(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  st_delete(&s);

  s = st_newc("ÁÉÍÓÚ", st_enc_utf8);

  up = st_capitalize(s);
  CHK_ALL(up, "Áéíóú", st_enc_utf8);
  st_delete(&up);

  up = st_ucfirst(s);
  CHK_ALL(up, "ÁÉÍÓÚ", st_enc_utf8);
  st_delete(&up);

  st_delete(&s);

  char buffer[12];
  st__char_upper("a", buffer, st_enc_ascii);
  ASSERT(strcmp(buffer, "A") == 0, "a uppercased");

  st__char_lower((char*)L"A", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"a") == 0, "a utf32 uppercased");

  st__char_lower((char*)L"Â", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"â") == 0, "ä utf32 uppercased");

  st__char_lower((char*)L"亜", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"亜") == 0, "? utf32 uppercased");

  st__char_upper((char*)L"ｅ", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"Ｅ") == 0, "? utf32 uppercased");

  st__char_lower((char*)L"Ｅ", buffer, st_enc_utf32be);
  ASSERT(wcscmp((wchar_t*)buffer, L"ｅ") == 0, "? utf32 uppercased");
}

void test_repeat() {
  string* s = st_newc(T_STR_03, st_enc_utf8);
  // str_repeat
  string* srepeat = st_repeat(s, 2);
  CHK_ALL(srepeat, T_STR_03_REP2, st_enc_utf8);
  st_delete(&srepeat);

  srepeat = st_repeat(s, 3);
  CHK_ALL(srepeat, T_STR_03_REP3, st_enc_utf8);
  st_delete(&srepeat);
  st_delete(&s);

  s = st_newc("", st_enc_ascii);
  srepeat = st_repeat(s, 3);
  CHK_ALL(srepeat, "", st_enc_ascii);
  st_delete(&srepeat);
  st_delete(&s);

  s = st_newc("a", st_enc_ascii);
  srepeat = st_repeat(s, 5);
  CHK_ALL(srepeat, "aaaaa", st_enc_ascii);
  st_delete(&srepeat);
  st_delete(&s);
}

void test_compare() {
  string* s = st_newc(T_STR_CMP1, st_enc_ascii);
  string* aux = st_newc(T_STR_CMP2, st_enc_ascii);

  ASSERT(st_compare(s, s) == 0, "cmp same string");
  ASSERT(st_compare(aux, aux) == 0, "cmp same string");
  ASSERT(st_compare(s, aux) > 0, "cmp T_STR_CMP1 vs T_STR_CMP2");
  ASSERT(st_compare(aux, s) < 0, "cmp T_STR_CMP2 vs T_STR_CMP1");

  st_copyc(&aux, T_STR_CMP3, st_enc_ascii);
  ASSERT(st_compare(s, aux) < 0, "cmp T_STR_CMP1 vs T_STR_CMP3");
  ASSERT(st_compare(aux, s) > 0, "cmp T_STR_CMP3 vs T_STR_CMP1");

  st_delete(&aux);
  st_delete(&s);

  s = st_newc("123", st_enc_ascii);
  aux = st_newc("1", st_enc_ascii);
  ASSERT(st_compare(s, aux) > 0, "cmp 123 vs 1");
  ASSERT(st_compare(aux, s) < 0, "cmp 1 vs 123");
  st_delete(&aux);
  st_delete(&s);

  s = st_newc("ab01cd", st_enc_ascii);
  aux = st_newc("01", st_enc_ascii);

  ASSERT(st_subcompare(s, aux, 2, 2) == 0, "substring compare");
  ASSERT(st_subcompare(s, aux, 2, 3) == 1, "substring compare");
  ASSERT(st_subcompare(s, aux, 2, 1) == -1, "substring compare");
  ASSERT(st_subcompare(s, aux, 3, 2) == 1, "substring compare");

  st_delete(&aux);
  st_delete(&s);
}

void test_hexbinhex() {
  // st_bin2hex
  string* s = st_newc("1001", st_enc_ascii);
  string* aux = st_bin2hex(s);

  CHK_ALL(aux, "31303031", st_enc_ascii);
  st_delete(&aux);

  // go
  st_copyc(&s, "1111", st_enc_ascii);
  aux = st_bin2hex(s);
  CHK_ALL(aux, "31313131", st_enc_ascii);
  // back
  string* aux2 = st_hex2bin(aux);
  CHK_ALL(aux2, "1111", st_enc_ascii);
  // cleanup
  st_delete(&aux);
  st_delete(&aux2);

  // once again!
  st_copyc(&s, "6e6f7420636f6d706c657465", st_enc_ascii);
  aux = st_hex2bin(s);
  CHK_ALL(aux, "not complete", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "31313131", st_enc_ascii);
  aux = st_hex2bin(s);
  CHK_ALL(aux, "1111", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "6578616d706c65206865782064617461", st_enc_ascii);
  aux = st_hex2bin(s);
  CHK_ALL(aux, "example hex data", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, T_STR_ASCII, st_enc_ascii);
  aux = st_bin2hex(s);
  aux2 = st_hex2bin(aux);

  CHK_ALL(aux2, T_STR_ASCII, st_enc_ascii);

  st_delete(&aux);
  st_delete(&aux2);

  st_delete(&s);
}

void test_from() {
  // st_base2number
  string* s = st_newc(T_STR_5BIN, st_enc_ascii);
  int d = st_base2number(s, 2);
  ASSERT(d == 5, "st_base2number is 5");
  st_delete(&s);

  s = st_dec2hex(10);
  CHK_ALL(s, "a", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex(47);
  CHK_ALL(s, "2f", st_enc_ascii);
  st_delete(&s);

  s = st_newc("111000111", st_enc_ascii);
  double dd = st_bin2dec(s);
  ASSERT(dd == 455, "bin2dec(111000111) == 455");
  st_delete(&s);

  s = st_newc("0111000111", st_enc_ascii);
  dd = st_bin2dec(s);
  ASSERT(dd == 455, "bin2dec(0111000111) == 0");
  st_delete(&s);

  s = st_newc("01234567", st_enc_ascii);
  dd = st_oct2dec(s);
  ASSERT(dd == 342391, "oct2dec(01234567) == 342391");
  st_delete(&s);

  s = st_newc("0567", st_enc_ascii);
  dd = st_oct2dec(s);
  ASSERT(dd == 375, "oct2dec(0567) == 375");
  st_delete(&s);

  s = st_newc("123abc", st_enc_ascii);
  dd = st_hex2dec(s);
  ASSERT(dd == 1194684, "hex2dec(123abc) == 1194684");
  st_delete(&s);

  s = st_newc("789DEF", st_enc_ascii);
  dd = st_hex2dec(s);
  ASSERT(dd == 7904751, "hex2dec(789DEF) == 7904751");
  st_delete(&s);

  s = st_dec2bin(10);
  CHK_ALL(s, "1010", st_enc_ascii);
  st_delete(&s);

  s = st_dec2bin((size_t)3.9505e3);
  CHK_ALL(s, "111101101110", st_enc_ascii);
  st_delete(&s);

  s = st_dec2oct(10);
  CHK_ALL(s, "12", st_enc_ascii);
  st_delete(&s);

  s = st_dec2oct((size_t)3950.5);
  CHK_ALL(s, "7556", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex(10);
  CHK_ALL(s, "a", st_enc_ascii);
  st_delete(&s);

  s = st_dec2hex((size_t)3950.5);
  CHK_ALL(s, "f6e", st_enc_ascii);
  st_delete(&s);

  // st_number2base

  string* binstr = st_number2base(5, 2);
  CHK_ALL(binstr, T_STR_5BIN, st_enc_ascii);
  st_delete(&binstr);
}

void test_append() {
  string* s = st_newc(T_STR_03, st_enc_utf8);

  st_append(&s, s);
  CHK_ALL(s, T_STR_03_REP2, st_enc_utf8);

  st_append(&s, s);
  CHK_ALL(s, T_STR_03_REP4, st_enc_utf8);

  st_delete(&s);

  s = st_newc(T_STR_03, st_enc_utf8);
  string* aux = st_concat(s, s);
  CHK_ALL(aux, T_STR_03_REP2, st_enc_utf8);

  st_delete(&aux);
  st_delete(&s);
}

void test_sub() {
  string* s;
  string* aux;

  s = st_newc(T_STR_03_REP4, st_enc_utf8);
  CHK_ALL(s, T_STR_03_REP4, st_enc_utf8);

  aux = st_sub(s, 0, 6);
  CHK_ALL(aux, T_STR_03, st_enc_utf8);

  st_delete(&aux);

  aux = st_sub(s, -12, 6);
  CHK_ALL(aux, T_STR_03_REP3, st_enc_utf8);

  st_delete(&aux);
  st_delete(&s);

  s = st_newc(T_STR_ASCII, st_enc_ascii);
  aux = st_sub(s, 0, 0);
  CHK_ALL(aux, T_STR_ASCII, st_enc_ascii);
  st_delete(&aux);
  st_delete(&s);

  s = st_newc((const char*)T_STR_ASCII_UTF32, st_enc_utf32be);
  CHK_ALL(s, T_STR_ASCII_UTF32, st_enc_utf32be);
  aux = st_sub(s, 0, 0);

  CHK_ALL(aux, (const char*)T_STR_ASCII_UTF32, st_enc_utf32be);
  st_delete(&aux);
  st_delete(&s);
}

void test_trim() {
  string* s;
  string* result;
  string* mask;

  s = st_newc("   123  ", st_enc_ascii);
  result = st_trim(s, 0, 3);

  CHK_ALL(result, "123", st_enc_ascii);

  st_delete(&s);
  st_delete(&result);

  mask = st_newc("0", st_enc_ascii);
  s = st_newc("000123x0", st_enc_ascii);

  result = st_trim(s, mask, 3);
  ASSERT(strcmp(result->value, "123x") == 0, "trim('000123x0') = '123x'");
  st_delete(&result);

  result = st_chop(s, mask);
  ASSERT(strcmp(result->value, "000123x") == 0, "chop('000123x0') = '000123x'");
  st_delete(&result);

  result = st_ltrim(s, mask);
  ASSERT(strcmp(result->value, "123x0") == 0, "ltrim('000123x0') = '123x0'");
  st_delete(&result);
  st_delete(&mask);

  mask = st_newc("9", st_enc_ascii);
  result = st_rtrim(s, mask);
  ASSERT(strcmp(result->value, "000123x0") == 0,
         "rtrim('000123x0') = '000123x0'");
  st_delete(&result);

  st_delete(&mask);
  st_delete(&s);
}

void test_utf8_lenc() {
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
}

void test_utf8_invalid() {
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
}

char buffer[256];
void char_itr_cb(const string* chr, st_len_t pos, string* src) {
  strcat(buffer, chr->value);
}

void wchar_itr_cb(const string* chr, st_len_t pos, string* src) {
  st_hexdump(chr->value, 8);
  wcscat((wchar_t*)buffer, (wchar_t*)chr->value);
}

void byte_itr_cb(st_uc_t chr, st_len_t pos, const string* src) {
  st_len_t i = strlen(buffer);
  buffer[i] = (char)chr;
  buffer[i + 1] = '\0';
}

void char_map_cb(string* chr, st_len_t pos, string* src) { chr->value[0] += 1; }

void test_itr_chars() {
  buffer[0] = '\0';
  string* str;
  string* nstr;

  str = st_newc(T_STR_ASCII, st_enc_ascii);
  buffer[0] = '\0';
  st_char_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, T_STR_ASCII) == 0, "concat char iteration");
  st_delete(&str);

  str = st_newc(T_STR_03_REP3, st_enc_utf8);

  buffer[0] = '\0';
  st_char_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, T_STR_03_REP3) == 0, "concat char iteration");

  buffer[0] = '\0';
  st_byte_iterator(str, byte_itr_cb);
  ASSERT(strcmp(buffer, T_STR_03_REP3) == 0, "concat char iteration");

  st_delete(&str);

  buffer[0] = '\0';
  buffer[1] = '\0';
  buffer[2] = '\0';
  buffer[3] = '\0';
  str = st_newc((const char*)L"abcñ", st_enc_utf32be);
  st_char_iterator(str, (st_char_itr_cb)wchar_itr_cb);

  ASSERT(wcscmp((const wchar_t*)buffer, L"abcñ") == 0, "concat wide iteration");
  st_delete(&str);

  //
  // line iterator
  //
  str = st_newc("01\n2\n345\n\n67\n89", st_enc_utf8);

  buffer[0] = '\0';
  st_line_iterator(str, (st_char_itr_cb)char_itr_cb);
  ASSERT(strcmp(buffer, "0123456789") == 0,
         "concat line iteration (remove newlines)");

  st_delete(&str);

  str = st_newc("abc", st_enc_ascii);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  CHK_ALL(nstr, "bcd", st_enc_ascii);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc("abc", st_enc_utf8);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  CHK_ALL(nstr, "bcd", st_enc_utf8);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc(T_STR_03, st_enc_utf8);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  CHK_ALL(nstr, "ojıp!㘃", st_enc_utf8);
  st_delete(&str);
  st_delete(&nstr);

  str = st_newc((const char*)L"ABC", st_enc_utf32be);

  nstr = st_char_map(str, (st_char_map_cb)char_map_cb);

  CHK_ALL(nstr, L"BCD", st_enc_utf32be);
  st_delete(&str);
  st_delete(&nstr);
}

// this not an actual test/assert just to be sure memory is ok
void test_shuffle() {
  string* s = st_newc("123456789", st_enc_ascii);
  string* aux = st_shuffle(s, s->length);
  st_delete(&s);
  st_delete(&aux);

  s = st_newc("1", st_enc_ascii);
  aux = st_shuffle(s, s->length);
  CHK_ALL(aux, "1", st_enc_ascii);
  st_delete(&s);
  st_delete(&aux);
}

void test_chr() {
  size_t num;
  string* s;

  s = st_chr(97, st_enc_ascii);
  CHK_ALL(s, "a", st_enc_ascii);
  assert(st_ord(s, 0) == 97);
  st_delete(&s);

  s = st_chr(98, st_enc_ascii);
  CHK_ALL(s, "b", st_enc_ascii);
  assert(st_ord(s, 0) == 98);
  st_delete(&s);

  s = st_chr(65, st_enc_ascii);
  CHK_ALL(s, "A", st_enc_ascii);
  assert(st_ord(s, 0) == 65);
  st_delete(&s);

  s = st_chr(65, st_enc_utf8);
  CHK_ALL(s, "A", st_enc_utf8);
  assert(st_ord(s, 0) == 65);
  st_delete(&s);

  s = st_chr(40845, st_enc_utf8);
  CHK_ALL(s, "龍", st_enc_utf8);
  assert(st_ord(s, 0) == 40845);
  st_delete(&s);

  s = st_chr(26159, st_enc_utf8);
  CHK_ALL(s, "是", st_enc_utf8);
  assert(st_ord(s, 0) == 26159);
  st_delete(&s);

  // found: http://en.wikibooks.org/wiki/Unicode/Character_reference/2F000-2FFFF
  s = st_chr(195038, st_enc_utf8);
  CHK_ALL(s, "軔", st_enc_utf8);
  assert(st_ord(s, 0) == 195038);
  st_delete(&s);

  /*
  s = st_chr(110011, st_enc_utf8);
  st_debug(s);
  st_delete(&s);
  s = st_newc("", st_enc_utf8);
  printf("%ld\n", st_ord(s, 0));
  exit(1);
  */

  s = st_newc(T_STR_03, st_enc_utf8);

  assert(st_ord(s, 0) == 110);
  assert(st_ord(s, 1) == 105);
  assert(st_ord(s, 2) == 241);
  assert(st_ord(s, 3) == 111);
  assert(st_ord(s, 4) == 32);
  assert(st_ord(s, 5) == 9731);
  // assert(st_ord(s, 6) == 0); out-of-range
  st_delete(&s);

  s = st_chr(9731, st_enc_utf8);
  CHK_ALL(s, "☃", st_enc_utf8);
  st_delete(&s);

  s = st_chr(241, st_enc_utf8);
  CHK_ALL(s, "ñ", st_enc_utf8);
  st_delete(&s);
}

void test_search() {
  string* replacement;
  string* hay = st_newc("0123456789", st_enc_ascii);
  string* ned = st_newc("5", st_enc_ascii);

  st_len_t i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 5, "st_pos = 5");

  i = st_pos(hay, ned, 0, 3);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("0X123456XX789", st_enc_ascii);
  ned = st_newc("XX", st_enc_ascii);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 8, "st_pos = 8");

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("ababababdababdababddx", st_enc_ascii);
  ned = st_newc("d", st_enc_ascii);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 8, "st_pos = 8");

  i = st_pos(hay, ned, 9, 0);
  ASSERT(i == 13, "st_pos = 13");

  i = st_pos(hay, ned, 14, 0);
  ASSERT(i == 18, "st_pos = 18");

  i = st_pos(hay, ned, 19, 0);
  ASSERT(i == 19, "st_pos = 19");

  i = st_pos(hay, ned, 20, 0);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&ned);
  ned = st_newc("fdusgeui", st_enc_ascii);
  i = st_pos(hay, ned, 20, 0);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&hay);
  st_delete(&ned);

  // UTF-8
  hay = st_newc(T_STR_03_REP4, st_enc_utf8);
  ned = st_newc("☃", st_enc_utf8);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 5, "st_pos = 5");

  i = st_pos(hay, ned, 6, 0);
  ASSERT(i == 11, "st_pos = 11");

  i = st_pos(hay, ned, 12, 0);
  ASSERT(i == 17, "st_pos = 17");

  i = st_pos(hay, ned, 18, 0);
  ASSERT(i == 23, "st_pos = 23");

  i = st_index_of(hay, ned, 1, 2);
  ASSERT(i == -1, "st_pos = -1");

  // st_start_with / st_end_with
  assert(st_start_with(hay, ned) == false);
  assert(st_end_with(hay, ned) == true);
  st_delete(&ned);

  ned = st_newc(T_STR_03, st_enc_utf8);
  assert(st_start_with(hay, ned) == true);
  assert(st_end_with(hay, ned) == true);

  st_delete(&ned);
  st_delete(&hay);

  // char_at
  string* s = st_newc(T_STR_03, st_enc_utf8);

  string* chr;
  chr = st_char_at(s, 0);
  CHK_ALL(chr, "n", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 1);
  CHK_ALL(chr, "i", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 2);
  CHK_ALL(chr, "ñ", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 3);
  CHK_ALL(chr, "o", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 4);
  CHK_ALL(chr, " ", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 5);
  CHK_ALL(chr, "☃", st_enc_utf8);
  st_delete(&chr);

  st_delete(&s);

  // utf32
  s = st_newc((char*)L"abc☃", st_enc_utf32be);

  chr = st_char_at(s, 3);
  CHK_ALL(chr, L"☃", st_enc_utf32be);
  st_delete(&chr);

  chr = st_char_at(s, 1);
  CHK_ALL(chr, L"b", st_enc_utf32be);
  st_delete(&chr);

  st_delete(&s);

  // st_rpos
  hay = st_newc(T_STR_03_REP4, st_enc_utf8);
  ned = st_newc("☃", st_enc_utf8);
  i = st_rpos(hay, ned, 0, hay->length);
  assert(i == 23);

  i = st_rpos(hay, ned, 0, -1);
  assert(i == 17);

  st_delete(&ned);
  st_delete(&hay);

  hay = st_newc("123", st_enc_utf8);
  ned = st_newc("2", st_enc_utf8);
  ASSERT(st_contains(hay, ned) == true, "123 contains 2");
  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("ABCDEFGHIJKLMNOPQRSTUVWXYZ", st_enc_ascii);
  ned = st_newc("m", st_enc_ascii);
  ASSERT(st_ipos(hay, ned, 0, 0) == 12, "m position is 12");
  ASSERT(st_irpos(hay, ned, 0, 0) == 12, "m position is 12 (rev)");

  chr = st_char_at(hay, 0);
  CHK_ALL(chr, "A", st_enc_ascii);
  st_delete(&chr);

  chr = st_char_at(hay, 12);
  CHK_ALL(chr, "M", st_enc_ascii);
  st_delete(&chr);

  st_delete(&hay);
  st_delete(&ned);

  //
  // delete
  //
  hay = st_newc("abcabc", st_enc_utf8);
  ned = st_newc("ab", st_enc_utf8);
  string* del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "cc", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("a", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "bcbc", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("c", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "abab", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("bc", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "aa", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("abc", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("abca", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, "bc", st_enc_utf8);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("1001000001", st_enc_utf8);
  ned = st_newc("1", st_enc_utf8);
  del = st_remove(hay, ned, 2, 2);
  CHK_ALL(del, "100000001", st_enc_utf8);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  //
  // delete UTF32
  //
  hay = st_newc((const char*)L"abcabc", st_enc_utf32be);
  ned = st_newc((const char*)L"ab", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"cc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"a", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"bcbc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"c", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"abab", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"bc", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"aa", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"abc", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"abca", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  CHK_ALL(del, L"bc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  //
  // delete example docs
  //
  string* haystack = st_newc("abcabcabc", st_enc_utf8);
  string* needle = st_newc("abc", st_enc_utf8);
  string* result = st_remove(haystack, needle, 2, -2);
  CHK_ALL(result, "abcabc", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&result);

  //
  // st_replace
  //
  haystack = st_newc("test", st_enc_utf8);
  needle = st_newc("e", st_enc_utf8);
  replacement = st_newc("b", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  CHK_ALL(result, "tbst", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  haystack = st_newc("abcabcabc", st_enc_utf8);
  needle = st_newc("abc", st_enc_utf8);
  replacement = st_newc("def", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  CHK_ALL(result, "defdefdef", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  haystack = st_newc("abcabcabca", st_enc_utf8);
  needle = st_newc("abca", st_enc_utf8);
  replacement = st_newc("def", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  CHK_ALL(result, "defbcdef", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  // grow
  haystack = st_newc("aaa", st_enc_ascii);
  needle = st_newc("a", st_enc_ascii);
  replacement = st_newc("abc", st_enc_ascii);
  result = st_replace(haystack, needle, replacement, 0);
  CHK_ALL(result, "abcabcabc", st_enc_ascii);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  //
  // st_spn
  //
  string* subject = st_newc("22222222aaaa bbb1111 cccc", st_enc_ascii);
  string* mask = st_newc("1234", st_enc_ascii);

  ASSERT(st_spn(subject, mask, 0, 0) == 8, "st_spn 1");
  ASSERT(st_spn(subject, mask, 2, 0) == 8, "st_spn 2");
  ASSERT(st_spn(subject, mask, 2, 3) == 6, "st_spn 3");

  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("this is the test string", st_enc_ascii);
  mask = st_newc("htes ", st_enc_ascii);
  // TODO! ASSERT(st_spn(subject, mask, 8, 30) == 1, "st_spn 1");
  // printf("%ld\n", st_spn(subject, mask, 8, 0));
  // exit(1);
  ASSERT(st_spn(subject, mask, 8, 0) == 19, "st_spn 4");
  ASSERT(st_spn(subject, mask, 0, 0) == 2, "st_spn 5");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("☃龍☃龍!☃龍☃龍!", st_enc_utf8);
  mask = st_newc("☃龍", st_enc_utf8);
  ASSERT(st_spn(subject, mask, 0, 0) == 4, "st_spn 6");
  ASSERT(st_spn(subject, mask, 5, 0) == 9, "st_spn 7");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("abcd", st_enc_utf8);
  mask = st_newc("apple", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 0, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("abcd", st_enc_utf8);
  mask = st_newc("banana", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 0, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("hello", st_enc_utf8);
  mask = st_newc("l", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 2, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("hello", st_enc_utf8);
  mask = st_newc("world", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 2, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  //
  // st_insert
  //

  subject = st_newc("hello", st_enc_utf8);
  string* ins = st_newc("world", st_enc_utf8);
  st_insert(&subject, ins, 0);
  CHK_ALL(subject, "worldhello", st_enc_utf8);

  st_insert(&subject, ins, 1);
  CHK_ALL(subject, "wworldorldhello", st_enc_utf8);

  st_insert(&subject, ins, -1);
  CHK_ALL(subject, "wworldorldhellworldo", st_enc_utf8);

  st_insert(&subject, ins, subject->length);
  CHK_ALL(subject, "wworldorldhellworldoworld", st_enc_utf8);

  st_delete(&subject);
  st_delete(&ins);
}

void test_ascii() {
  assert(st_ascii_char_size_safe("\xFF") == -1);
  assert(st_ascii_char_size_safe("x") == 1);
  assert(st_ascii_char_size_safe("z") == 1);

  ASSERT(st_is_ascii(T_STR_ASCII) == true, "ascii is ascii -> true!");
  ASSERT(st_is_ascii(T_STR_03) == false, "utf8 is ascii -> false!");

  ASSERT(st_ascii_valid_codepoint(5) == true, "in range codepoint")
  ASSERT(st_ascii_valid_codepoint(300) == false, "out of range codepoint");

  st_size_t bytes;
  ASSERT(st_ascii_length("abc", &bytes) == 3, "length 3");
  ASSERT(bytes == 3, "size 3");
  ASSERT(st_ascii_char_size("a") == 1, "ascii char size is always 1");
}

void test_utf8() {
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
}

void test_encoding_go_back(int* utf32p, char* utf8) {
  string* src = st_newc(utf8, st_enc_utf8);

  string* go = st_to_utf32(src);
  assert(wcscmp((const wchar_t*)go->value, (const wchar_t*)utf32p) == 0);

  string* back = st_to_utf8(go);
  assert(back != 0);

  assert(strcmp(back->value, src->value) == 0);

  st_delete(&src);
  st_delete(&go);
  st_delete(&back);
}

void test_encoding() {
  // greek
  test_encoding_go_back(L"ο Δικαιοπολις εν αγρω εστιν",
                        "ο Δικαιοπολις εν αγρω εστιν");
  // misc
  test_encoding_go_back(L"Iñtërnâtiônàlizætiøn", "Iñtërnâtiônàlizætiøn");

  // Jap/arab etc..
  test_encoding_go_back(L"AÀΑ╬豈Ａꊠ黠だ➀ጀะڰЯ0123456789",
                        "AÀΑ╬豈Ａꊠ黠だ➀ጀะڰЯ0123456789");

  assert(st_utf8_length("123456789也不是可运行的程序１２３４５６７８９", 0) ==
         27);
  test_encoding_go_back(L"123456789也不是可运行的程序１２３４５６７８９",
                        "123456789也不是可运行的程序１２３４５６７８９");

  test_encoding_go_back(
      L"☃是龍𩸽𠜎 𠜱 𠝹 𠱓 𠱸 𠲖 𠳏 𠳕 𠴕 𠵼 𠵿 𠸎 𠸏 𠹷 𠺝 𠺢 𠻗 𠻹 𠻺 𠼭 𠼮 "
      L"𠽌 𠾴 𠾼 𠿪 𡁜 𡁯 𡁵 𡁶 𡁻 𡃁 𡃉 𡇙 𢃇 𢞵 𢫕 𢭃 𢯊 𢱑 𢱕 𢳂 𢴈 𢵌 𢵧 "
      L"𢺳 𣲷 𤓓 𤶸 𤷪 𥄫 𦉘 𦟌 𦧲 𦧺 𧨾 𨅝 𨈇 𨋢 𨳊 𨳍 𨳒 𩶘",
      "☃是龍𩸽𠜎 𠜱 𠝹 𠱓 𠱸 𠲖 𠳏 𠳕 𠴕 𠵼 𠵿 𠸎 𠸏 𠹷 𠺝 𠺢 𠻗 𠻹 𠻺 𠼭 𠼮 "
      "𠽌 𠾴 𠾼 𠿪 𡁜 𡁯 𡁵 𡁶 𡁻 𡃁 𡃉 𡇙 𢃇 𢞵 𢫕 𢭃 𢯊 𢱑 𢱕 𢳂 𢴈 𢵌 𢵧 𢺳 "
      "𣲷 𤓓 𤶸 𤷪 𥄫 𦉘 𦟌 𦧲 𦧺 𧨾 𨅝 𨈇 𨋢 𨳊 𨳍 𨳒 𩶘");
}

void test_internal() {
  string* s;
  char* start;
  char* end;

  st_len_t start_pos = 0;
  st_len_t end_pos = 0;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 0);
  assert(end_pos == 10);

  start_pos = 1;
  end_pos = -1;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 1);
  assert(end_pos == 8);

  start_pos = 5;
  end_pos = 5;
  st__calc_range(10, &start_pos, &end_pos);
  assert(start_pos == 5);
  assert(end_pos == 5);

  start_pos = 9;
  end_pos = 0;
  st__calc_range(21, &start_pos, &end_pos);
  assert(start_pos == 9);
  assert(end_pos == 12);

  start_pos = -2;
  end_pos = 2;
  st__calc_range(21, &start_pos, &end_pos);
  assert(start_pos == 19);
  assert(end_pos == 2);

  s = st_newc("0123456789", st_enc_ascii);

  assert(st__get_char_offset(s, 0) == s->value);
  assert(st__get_char_offset(s, 5) - 5 == s->value);
  assert(st__get_char_offset(s, 3) - 3 == s->value);
  assert(st__get_char_offset(s, 9) - 9 == s->value);

  assert(st__get_char_offset(s, -5) - 5 == s->value);
  assert(st__get_char_offset(s, -9) - 1 == s->value);
  assert(st__get_char_offset(s, -1) - 9 == s->value);
  /*
    st__get_char_range(s, 0, 5, &start, &end);
    assert(start == s->value);
    assert(end == s->value + 5);


    st__get_char_range(s, -2, 2, &start, &end);
    assert(start == s->value + 8);
    assert(end == s->value + 10);


    st__get_char_range(s, -2, -1, &start, &end);
    assert(start == s->value + 8);
    assert(end == s->value + 9);
  */

  st_delete(&s);

  // st__mempbrk

  // Check 1
  string* x;
  string* y;
  char* ret;
  x = st_newc("I will not have my fwends widiculed by the common soldiewy",
              st_enc_ascii);
  y = st_newc("zpm1", st_enc_ascii); // Finds the 'm'
  ret = st__mempbrk(x->value, y->value);
  assert(ret == &x->value[16]); //, "Simple strpbrk()" );
  st_delete(&x);
  st_delete(&y);

  // Check 2
  x = st_newc("Not bad for a little fur ball. You! Stay here.", st_enc_ascii);
  y = st_newc("zx", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == NULL); //, "Letters not found");
  st_delete(&x);
  st_delete(&y);

  // Check 3 (boundary condition)
  x = st_newc("", st_enc_ascii);
  y = st_newc("zx", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "String to search empty" );
  st_delete(&x);
  st_delete(&y);

  // Check 4 (boundary condition)
  x = st_newc("zx", st_enc_ascii);
  y = st_newc("", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "Empty search string" );
  st_delete(&x);
  st_delete(&y);

  // Check 5 (boundary condition)
  x = st_newc("", st_enc_ascii);
  y = st_newc("", st_enc_ascii);
  ret = st__mempbrk(x->value, y->value);
  assert(ret == 0); //, "Both strings empty" );
  st_delete(&x);
  st_delete(&y);
}

void test_utils() {
  assert(st_length("Iñtërnâtiônàlizætiøn", st_enc_utf8) == 20);
}

extern void test_justify() {
  string* x;
  string* z;

  x = st_newc("abc", st_enc_ascii);

  z = st_center(x, 5, 0);
  CHK_ALL(z, " abc ", st_enc_ascii);
  st_delete(&z);

  z = st_left(x, 5, 0);
  CHK_ALL(z, "abc  ", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 5, 0);
  CHK_ALL(z, "  abc", st_enc_ascii);
  st_delete(&z);

  z = st_center(x, 6, 0);
  CHK_ALL(z, " abc  ", st_enc_ascii);
  st_delete(&z);

  z = st_left(x, 6, 0);
  CHK_ALL(z, "abc   ", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 6, 0);
  CHK_ALL(z, "   abc", st_enc_ascii);
  st_delete(&z);

  z = st_right(x, 2, 0);
  CHK_ALL(z, "abc", st_enc_ascii);
  st_delete(&z);

  string* padstr = st_newc("x", st_enc_ascii);
  z = st_right(x, 6, padstr);
  CHK_ALL(z, "xxxabc", st_enc_ascii);
  st_delete(&z);
  st_delete(&padstr);
  st_delete(&x);

  x = st_newc("是龍", st_enc_utf8);
  padstr = st_newc("☃", st_enc_utf8);

  z = st_right(x, 10, padstr);
  CHK_ALL(z, "☃☃☃☃☃☃☃☃是龍", st_enc_utf8);
  st_delete(&z);
  st_delete(&padstr);
  st_delete(&x);
}
