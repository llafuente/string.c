/*
* this file is CPP because I use clang address-analyzer
* and dont run in c11 (dont ask me why! i don't know)
*/

#include "stringc.h"

#define T_STR_01 "hello world!"
#define T_STR_02 "hello my friend i'm even larger!!"
#define T_STR_03 "niño ☃"
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

#define CHK_LEN(src) assert(src->length == strlen(src->value));
#define CHK_VAL(src, dst) assert(0 == strcmp(src->value, dst));

#define CHK_ALL(src, dst, enc) \
printf("src = %s [%ld][%ld][%lu]\n", src->value, src->length, src->used, src->capacity); \
printf("dst = %s [%ld][%lu]\n", dst, st_length(dst, enc), st_capacity(dst, enc)); \
assert(0 == strcmp(src->value, dst)); \
assert(src->length == st_length(dst, enc)); \
assert(src->used == st_capacity(dst, enc)); \
assert(src->capacity >= src->used); \
assert(src->encoding == enc); \


#define RUN_TEST(test) \
printf("%s\n", #test); \
test_ ## test(); \


extern void test_memory_funcs();
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
extern void test_capitalize();
extern void test_shuffle();
extern void test_chr();
extern void test_search();

int main(int argc, const char * argv[]) {

  RUN_TEST(memory_funcs);
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
  RUN_TEST(capitalize);
  RUN_TEST(shuffle);
  RUN_TEST(chr);
  RUN_TEST(search);

  st_cleanup();
  printf("OK\n");

  return 0;
}

void test_memory_funcs() {
  //
  // memory
  //
  string* s = st_new(2, st_enc_ascii);

  assert(s->used == 0);
  assert(s->length == 0);
  assert(s->capacity == 3); // 3 because is null-terminated
  assert(s->value[0] == '\0');


  //assert(is_utf8(T_STR_02) == 0);
  //assert(is_utf8(T_STR_03_REP4) == 0);

  //st_resize(&s, 50);
  st_copyc(&s, T_STR_01, st_enc_ascii);
  assert(s->capacity == 13);
  CHK_ALL(s, T_STR_01, st_enc_ascii);

  st_copyc(&s, T_STR_02, st_enc_ascii);
  CHK_ALL(s, T_STR_02, st_enc_ascii);

  st_copyc(&s, T_STR_03, st_enc_utf8);
  CHK_ALL(s, T_STR_03, st_enc_utf8);
  st_delete(&s);


  s = st_newc(T_STR_03, st_enc_utf8);
  CHK_ALL(s, T_STR_03, st_enc_utf8);
  st_delete(&s);
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
}

char buffer[256];
void itr_callback(string* chr, st_len_t pos, string* src) {
  strcat(buffer, chr->value);
}

void test_compare() {
  string* s = st_newc(T_STR_CMP1, st_enc_ascii);
  CHK_ALL(s, T_STR_CMP1, st_enc_ascii);

  string* aux = st_newc(T_STR_CMP2, st_enc_ascii);
  CHK_ALL(aux, T_STR_CMP2, st_enc_ascii);

  assert(st_compare(s, s) == 0);
  assert(st_compare(aux, aux) == 0);
  assert(st_compare(s, aux) > 0);

  st_copyc(&aux, T_STR_CMP3, st_enc_ascii);
  CHK_ALL(aux, T_STR_CMP3, st_enc_ascii);

  assert(st_compare(s, aux) < 0);

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
  st_delete(&s);
}

void test_from() {
  // st_base2number
  string* s = st_newc(T_STR_5BIN, st_enc_ascii);
  int d = st_base2number(s, 2);
  assert(d == 5);
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
  string* s = st_newc(T_STR_03_REP4, st_enc_utf8);
  CHK_ALL(s, T_STR_03_REP4, st_enc_utf8);

  string* aux = st_sub(s, 0, 6);
  CHK_ALL(aux, T_STR_03, st_enc_utf8);

  st_delete(&aux);

  aux = st_sub(s, -12, 6);
  CHK_ALL(aux, T_STR_03_REP3, st_enc_utf8);

  st_delete(&aux);
  st_delete(&s);
}

void test_trim() {
  string* s = st_new(10, st_enc_ascii);
  st_copyc(&s, "   123  ", st_enc_ascii);
  string* aux = st_trim(s, 0, 3);

  CHK_ALL(aux, "123", st_enc_ascii);

  st_delete(&s);
  st_delete(&aux);


  s = st_new(10, st_enc_ascii);
  string* mask = st_newc("0", st_enc_ascii);
  st_copyc(&s, "000123x0", st_enc_ascii);
  aux = st_trim(s, mask, 3);

  assert(strcmp(aux->value, "123x") == 0);

  st_delete(&mask);
  st_delete(&s);
  st_delete(&aux);
}

void test_utf8_lenc() {
  size_t cap;
  assert(string_utf8_lenc(T_STR_01, &cap) == 12);
  assert(strlen(T_STR_01) == cap);
  assert(string_utf8_lenc(T_STR_02, &cap) == 33);
  assert(strlen(T_STR_02) == cap);
  assert(string_utf8_lenc(T_STR_03, &cap) == 6);
  assert(strlen(T_STR_03) == cap);
  assert(string_utf8_lenc(T_STR_03_REP2, &cap) == 12);
  assert(strlen(T_STR_03_REP2) == cap);
  assert(string_utf8_lenc(T_STR_03_REP3, &cap) == 18);
  assert(strlen(T_STR_03_REP3) == cap);
  assert(string_utf8_lenc(T_STR_03_REP4, &cap) == 24);
  assert(strlen(T_STR_03_REP4) == cap);
}

void test_utf8_invalid() {
  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_1, strlen(T_STR_UTF8_1)) == 0);
  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_2, strlen(T_STR_UTF8_2)) == 0);
  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_3, strlen(T_STR_UTF8_3)) == 0);
  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_4, strlen(T_STR_UTF8_4)) == 0);

  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_5, strlen(T_STR_UTF8_5)) == T_STR_UTF8_5 + 1);
  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_6, strlen(T_STR_UTF8_6)) == T_STR_UTF8_6 + 5);

  assert(string_utf8_invalid((const unsigned char *) T_STR_UTF8_7, strlen(T_STR_UTF8_7)) == 0);
}

void test_itr_chars() {

  string* str = st_newc(T_STR_03_REP3, st_enc_utf8);
  st_char_iterator(str, itr_callback);
  st_delete(&str);

  assert(strcmp(buffer, T_STR_03_REP3) == 0);

}

void test_capitalize() {
  return;
  string* str = st_newc(T_STR_CAP_1, st_enc_ascii);
  string_capitalize(str);
  st_debug(str);
  CHK_VAL(str, T_STR_CAP_T1);
  st_delete(&str);

  str = st_newc(T_STR_CAP_2, st_enc_ascii);
  string_capitalize(str);
  st_debug(str);
  CHK_VAL(str, T_STR_CAP_T2);
  st_delete(&str);

  str = st_newc(T_STR_CAP_3, st_enc_ascii);
  string_capitalize(str);
  st_debug(str);
  CHK_VAL(str, T_STR_CAP_T3);
  st_delete(&str);
}

// this not an actual test/assert just to be sure memory is ok
void test_shuffle() {
  string* s = st_newc("123456789", st_enc_ascii);
  string* aux = st_shuffle(s, s->length);
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

  s = st_newc(T_STR_03, st_enc_utf8);

  assert(st_ord(s, 0) == 110);
  assert(st_ord(s, 1) == 105);
  assert(st_ord(s, 2) == 241);
  assert(st_ord(s, 3) == 111);
  assert(st_ord(s, 4) == 32);
  assert(st_ord(s, 5) == 9731);
  //assert(st_ord(s, 6) == 0); out-of-range
  st_delete(&s);

  s = st_chr(9731, st_enc_utf8);
  CHK_ALL(s, "☃", st_enc_utf8);
  st_delete(&s);

  s = st_chr(241, st_enc_utf8);
  CHK_ALL(s, "ñ", st_enc_utf8);
  st_delete(&s);
}


void test_search() {
  string* hay = st_newc("0123456789", st_enc_ascii);
  string* ned = st_newc("5", st_enc_ascii);

  st_len_t i = st_pos(hay, ned, 0);

  assert(i == 5);

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("0X123456XX789", st_enc_ascii);
  ned = st_newc("XX", st_enc_ascii);

  i = st_pos(hay, ned, 0);
  assert(i == 8);

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("ababababdababdababddx", st_enc_ascii);
  ned = st_newc("d", st_enc_ascii);

  i = st_pos(hay, ned, 0);
  assert(i == 8);

  i = st_pos(hay, ned, 9);
  assert(i == 13);

  i = st_pos(hay, ned, 14);
  assert(i == 18);

  i = st_pos(hay, ned, 19);
  assert(i == 19);

  i = st_pos(hay, ned, 20);
  assert(i == -1);

  st_delete(&ned);
  ned = st_newc("fdusgeui", st_enc_ascii);
  i = st_pos(hay, ned, 20);
  assert(i == -1);

  st_delete(&hay);
  st_delete(&ned);

}
