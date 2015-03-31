#include "string-bin2hex.h"

string* string_bin2hex(const string* old) {
  string *result = string_new(old->length * 2);
  size_t i, j;
  char* str = result->value;
  for (i = j = 0; i < old->length; i++) {
    *(str + j) = hexconvtab[old->value[i] >> 4];
    ++j;
    *(str + j) = hexconvtab[old->value[i] & 15];
    ++j;
  }
  result->value[j] = '\0';
  result->length = j;
  return result;
}
