#include "stringc.h"

string* string_sub(string* str, int start, int end) {
  assert(end < str->length);
  assert(end > -str->length);

  size_t len = 0;
  if (start < 0) {

    len -= start;
    len += end;
  } else {
    len = end - start;
  }

  //printf("string %s start %d end %d\n", str->value, start, end);
  //printf("len %zu\n", len);

  string* out = string_new(len);

  size_t pos,
  idx = 0;
  while (start < 0) {
    pos = str->length + start;
    //printf("pos %zu", pos);
    out->value[idx++] = str->value[pos];

    ++start;
  }

  //printf("start %d %c\n", start, str->value[start]);
  //printf("idx %zu\n", idx);

  while (start != end) {
    out->value[idx++] = str->value[start++];
  }
  out->length = idx;

  //printf("idx %zu\n", idx);

  string_zeronull(out);

  return out;
}
