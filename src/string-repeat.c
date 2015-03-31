#include "stringc.h"

string* string_repeat(string* input, size_t mult) {
  string *result; /* Resulting string */
  size_t result_len; /* Length of the resulting string */
  /* Don't waste our time if it's empty */
  /* ... or if the multiplier is zero */
  if (input->length == 0 || mult == 0) {
    return string_new((size_t) 0);
  }

  /* Initialize the result string */
  result_len = input->length * mult;
  result = string_new(result_len);
  /* Heavy optimization for situations where input string is 1 byte long */
  if (input->length == 1) {
    memset(result->value, *(input->value), mult);
  } else {
    char *s, *e, *ee;
    //TODO review: ptrdiff_t l=0;
    size_t l=0;
    memcpy(result->value, input->value, input->length);
    s = result->value;
    e = result->value + input->length;
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
