// http://www.unicode.org/reports/tr10/
// http://www.unicode.org/Public/UCA/latest/allkeys.txt

#include "stringc.h"

#define STRING_IS_LOW_ASCII(c) (c > 96) && (c < 123)
#define STRING_LOW_ASCCI(c) c-=32;

#define STRING_IS_UP_ASCII(c) (c > 64) && (c < 97)
#define STRING_UP_ASCCI(c) c-=32;

//ascii
void string_char_up(string* str, st_len_t pos) {
  assert(str->length > pos);

  char* val = str->value;
  char c = val[pos];

  if ((c > 96) && (c < 123)) {
    val[pos] -= 32 ;
  }
}

void string_char_low(string* str, st_len_t pos) {
  assert(str->length > pos);

  char* val = str->value;
  char c = val[pos];

  if ((c > 64) && (c < 97)) {
    val[pos] += 32 ;
  }
}

// http://pastebin.com/fuw4Uizk
void string_capitalize(string* str) {
  assert(str->encoding == st_enc_ascii);

  if (STRING_IS_UP_ASCII(str->value[0])) {
    STRING_LOW_ASCCI(str->value[0]);
  }

  STRING_LOOP_START(str, itr, 1) {
    if (STRING_IS_UP_ASCII(*itr)) {
      STRING_LOW_ASCCI(itr);
    }
  }

}

string* string_ucfirst(string* src) {
  return 0;
}

void st_upper_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;

  // ascci do direct translation
  if (uc < 127) {
    if ((uc > 96) && (uc < 123)) {
      *character->value -= 32 ;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st__utf8c_to_utf32cp((st_uc_t*) character->value);
  cp = st_utf32_uppercase(cp);
  st_len_t zero_null_pos = st__utf32cp_to_utf8c(cp, (st_uc_t*) character->value);
  character->value[zero_null_pos] = '\0';
}

// TODO improve performance
string* st_upper(string* src) {
  return st_char_map(src, st_upper_cb);
}

string* string_lcfirst(string* src) {
  return 0;
}

void st_lower_cb(string* character, st_len_t pos, const string* src) {
  st_uc_t uc = *character->value;

  // ascci do direct translation
  if (uc < 127) {
    if ((uc > 64) && (uc < 97)) {
      *character->value += 32 ;
    }
    return;
  }

  // utf8 -> utf32
  st_uc4_t cp = st__utf8c_to_utf32cp((st_uc_t*) character->value);
  cp = st_utf32_lowercase(cp);
  st_len_t zero_null_pos = st__utf32cp_to_utf8c(cp, (st_uc_t*) character->value);
  character->value[zero_null_pos] = '\0';
}

// TODO improve performance
string* st_lower(string* src) {
  return st_char_map(src, st_lower_cb);
}

string* string_swapcase(string* src) {
  return 0;
}
