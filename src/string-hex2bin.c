#include "string-type.h"

/*
* @credits php-src
*/
string *string_hex2bin(string *src) {
  size_t target_length = src->length >> 1;
  char* src_val = src->value;
  string *out = string_new(target_length);
  unsigned char *out_val = (unsigned char *)out->value;
  size_t i, j;
  unsigned char c, d;
  for (i = j = 0; i < target_length; i++) {
    c = src_val[j++];
    if (c >= '0' && c <= '9') {
      d = (c - '0') << 4;
    } else if (c >= 'a' && c <= 'f') {
      d = (c - 'a' + 10) << 4;
    } else if (c >= 'A' && c <= 'F') {
      d = (c - 'A' + 10) << 4;
    } else {
      string_delete(&out);
      return 0;
    }
    c = src_val[j++];
    if (c >= '0' && c <= '9') {
      d |= c - '0';
    } else if (c >= 'a' && c <= 'f') {
      d |= c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      d |= c - 'A' + 10;
    } else {
      string_delete(&out);
      return 0;
    }
    out_val[i] = d;
  }
  out_val[i] = '\0';
  out->length = i;
  return out;
}
