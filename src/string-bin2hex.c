#include "stringc.h"

static char string_hexconvtab[] = "0123456789abcdef";

string* string_bin2hex(const string* old) {
  string *result = string_new(old->length * 2);
  size_t i, j;
  char* str = result->value;
  for (i = j = 0; i < old->length; i++) {
    *(str + j) = string_hexconvtab[old->value[i] >> 4];
    ++j;
    *(str + j) = string_hexconvtab[old->value[i] & 15];
    ++j;
  }
  result->value[j] = '\0';
  result->length = j;
  return result;
}
