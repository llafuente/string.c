#include "stringc.h"

string* string_repeat(string* src, size_t x) {
  string *result; /* Resulting string */
  size_t result_len; /* Length of the resulting string */
  string_len_t src_len = src->length; // @cache
  /* Don't waste our time if it's empty */
  /* ... or if the multiplier is zero */
  if (src_len == 0 || x == 0) {
    return string_new((size_t) 0);
  }

  /* Initialize the result string */
  result_len = src_len * x;
  result = string_new(result_len);
  /* Heavy optimization for situations where src string is 1 byte long */
  if (src_len == 1) {
    memset(result->value, *(src->value), x);
  } else {
    char *s, *e, *ee;
    //TODO review: ptrdiff_t l=0;
    size_t l=0;
    memcpy(result->value, src->value, src_len);
    s = result->value;
    e = result->value + src_len;
    ee = result->value + result_len;
    while (e<ee) {
      l = (e-s) < (ee-e) ? (e-s) : (ee-e);
      memmove(e, s, l);
      e += l;
    }
  }
  result->value[result_len] = '\0';
  return result;
}
