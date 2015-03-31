#include "stringc.h"

int string_compare(string* a, string* b) {
  if (a->length > b->length) {
    return 1;
  }

  if (a->length < b->length) {
    return -1;
  }

  const char *pa = a->value,
    *pb = b->value;

  while(*pa && (*pa==*pb)) {
    ++pa;
    ++pb;
  }

  return *(const unsigned char*)pa - *(const unsigned char*)pb;
}
