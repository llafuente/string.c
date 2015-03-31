#include "stringc.h"

void string_append(string** out, string* src) {
  //printf("string_append %p - %p\n", *out, src);

  string* cache = *out;

  string_len_t src_len = src->length;
  string_len_t total = src_len + cache->length;
  size_t cap = cache->capacity;

  if (cap < total) {
    // out will be reallocated if src is the same we should use it.
    bool same = cache == src;

    assert(*out == src);
    string_resize(out, total);
    cache = *out;
    if (same) {
      src = cache;
    }
  }


  memcpy(cache->value + cache->length, src->value, src_len);
  cache->length += src_len;

  cache->value[cache->length] = '\0';
}
