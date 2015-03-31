#include <stdint.h>
#include <stddef.h>

#ifndef __STRINGC_TYPE__
#define __STRINGC_TYPE__

typedef long int string_len_t;

typedef enum {
  ascii,
  utf8
} charset_t;

typedef struct  {
  string_len_t length;
  size_t capacity;
  charset_t charset;
  char value[];
} string;

#endif
