#include "stringc.h"

void string_zeronull(string* str) {
  str->value[str->length] = '\0';
}

void string_debug(string* str) {
  printf("string_debug @%p, length[%zu] size[%zu]\n", str, str->length, str->capacity);

  char *p = str->value;
  size_t size = str->capacity;
  printf("\nhexadecimal\n");
  int n;
  for (n = 0; n < size; ++n) {
    printf("| %2.2x ", (*p) & 0xff);
    ++p;
  }
  printf("\nchar by char\n");
  p = str->value;

  for (n = 0; n < size; ++n) {
    printf("| %c  ", *p ? *p : ' ');
    ++p;
  }
  printf("\nprintf %s\n", str->value);
}
