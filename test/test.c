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
printf("src = %s [%d][%zu][%d]\n", src->value, src->length, src->used, src->capacity); \
printf("dst = %s [%zu][%zu]\n", dst, string_length(dst, enc), string_capacity(dst, enc)); \
assert(0 == strcmp(src->value, dst)); \
assert(src->length == string_length(dst, enc)); \
assert(src->used == string_capacity(dst, enc)); \
assert(src->capacity >= src->used); \

extern void test_memory_funcs();
extern void test_repeat();
extern void test_itr_chars();
extern void test_utf8_invalid();
extern void test_utf8_lenc();
extern void test_trim();
extern void test_capitalize();

int main(int argc, const char * argv[]) {



/*
  string* s;
  string_copyc(&s, T_STR_CMP1);
  aux = string_newc(T_STR_CMP2);

  assert(string_compare(s, s) == 0);
  assert(string_compare(aux, aux) == 0);
  assert(string_compare(s, aux) > 0);
  string_copyc(&aux, T_STR_CMP3);
  assert(string_compare(s, aux) < 0);

  string_delete(&aux);

  // string_bin2hex
  // WTF?!
  string_copyc(&s, "1001");
  aux = string_bin2hex(s);
  CHK_LEN(aux);
  CHK_VAL(aux, "31303031");
  string_delete(&aux);

  string_copyc(&s, "1111");
  aux = string_bin2hex(s);
  CHK_VAL(aux, "31313131");
  aux2 = string_hex2bin(aux);
  CHK_VAL(aux2, "1111");
  string_delete(&aux);
  string_delete(&aux2);

  string_copyc(&s, "6e6f7420636f6d706c657465");
  aux = string_hex2bin(s);
  CHK_VAL(aux, "not complete");
  string_delete(&aux);

  string_copyc(&s, "31313131");
  aux = string_hex2bin(s);
  CHK_VAL(aux, "1111");
  string_delete(&aux);

  string_copyc(&s, "6578616d706c65206865782064617461");
  aux = string_hex2bin(s);
  CHK_VAL(aux, "example hex data");
  string_delete(&aux);


  // string_from_base

  string_copyc(&s, T_STR_5BIN);
  int d = string_from_base(s, 2);
  assert(d == 5);

  // string_from_number

  string* binstr = string_from_number(5, 2);
  CHK_VAL(binstr, T_STR_5BIN);
  string_delete(&binstr);

  string_copyc(&s, T_STR_03);

  string_append(&s, s);
  CHK_VAL(s, T_STR_03_REP2);

  string_append(&s, s);
  CHK_VAL(s, T_STR_03_REP4);

  aux = string_sub(s, 0, 4);
  //string_debug(aux);
  string_delete(&aux);
  aux = string_sub(s, -10, 4);
  //string_debug(aux);
  string_delete(&aux);

  string_delete(&s);




  s = string_newc("123456789");
  aux = string_shuffle(s, s->length);
  string_delete(&s);
  string_delete(&aux);
*/


  test_memory_funcs();
  test_repeat();
  test_utf8_lenc();
  test_utf8_invalid();
  test_itr_chars();
  test_capitalize();

  string_cleanup();
  printf("OK\n");

  return 0;
}

void test_memory_funcs() {
  //
  // memory
  //
  string* s = string_new(2, string_enc_ascii);

  assert(s->used == 0);
  assert(s->length == 0);
  assert(s->capacity == 3); // 3 because is null-terminated
  assert(s->value[0] == '\0');


  //assert(is_utf8(T_STR_02) == 0);
  //assert(is_utf8(T_STR_03_REP4) == 0);

  //string_resize(&s, 50);
  string_copyc(&s, T_STR_01);
  assert(s->capacity == 13);
  CHK_ALL(s, T_STR_01, string_enc_ascii);

  string_copyc(&s, T_STR_02);
  CHK_ALL(s, T_STR_02, string_enc_ascii);

  string_copyc(&s, T_STR_03, string_enc_utf8);
  CHK_ALL(s, T_STR_03, string_enc_utf8);
  string_delete(&s);


  s = string_newc(T_STR_03, string_enc_utf8);
  CHK_ALL(s, T_STR_03, string_enc_utf8);
  string_delete(&s);
}

void test_repeat() {
  string* s = string_newc(T_STR_03, string_enc_utf8);
  // str_repeat
  string* srepeat = string_repeat(s, 2);
  CHK_ALL(srepeat, T_STR_03_REP2, string_enc_utf8);
  string_delete(&srepeat);

  srepeat = string_repeat(s, 3);
  CHK_VAL(srepeat, T_STR_03_REP3);
  string_delete(&srepeat);
  string_delete(&s);
}

char buffer[256];
void itr_callback(string* chr, string_len_t pos, string* src) {
  strcat(buffer, chr->value);
}

void test_trim() {
  string* s = string_new(10);
  string_copyc(&s, "   123  ");
  string* aux = string_trim(s, 0, 3);

  CHK_VAL(aux, "123");

  string_delete(&s);
  string_delete(&aux);


  s = string_new(10);
  string* mask = string_newc("0", string_enc_ascii);
  string_copyc(&s, "000123x0");
  aux = string_trim(s, mask, 3);

  assert(strcmp(aux->value, "123x") == 0);

  string_delete(&mask);
  string_delete(&s);
  string_delete(&aux);
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

  string* str = string_newc(T_STR_03_REP3, string_enc_utf8);
  string_itr_chars(str, itr_callback);
  string_delete(&str);

  assert(strcmp(buffer, T_STR_03_REP3) == 0);

}

void test_capitalize() {
  return;
  string* str = string_newc(T_STR_CAP_1, string_enc_ascii);
  string_capitalize(str);
  string_debug(str);
  CHK_VAL(str, T_STR_CAP_T1);
  string_delete(&str);

  str = string_newc(T_STR_CAP_2, string_enc_ascii);
  string_capitalize(str);
  string_debug(str);
  CHK_VAL(str, T_STR_CAP_T2);
  string_delete(&str);

  str = string_newc(T_STR_CAP_3, string_enc_ascii);
  string_capitalize(str);
  string_debug(str);
  CHK_VAL(str, T_STR_CAP_T3);
  string_delete(&str);
}
