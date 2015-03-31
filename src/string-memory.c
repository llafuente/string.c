#include "stringc.h"

string* _string_new(size_t len, charset_t charset) {
  size_t size = len + 1; // null terminated!

  string* s = (string*) __string_allocator(sizeof(string) + size * sizeof(char));

  s->length = 0;
  s->capacity = size;
  s->value[0] = '\0';
  s->charset = charset;

  return s;
}


string* _string_newc(const char* chars, charset_t charset) {
  size_t size = strlen(chars); // null terminated!

  string* s = (string*) __string_allocator(sizeof(string) + (size + 1) * sizeof(char));

  s->length = size;
  s->capacity = size + 1;
  memcpy(s->value, chars, size);
  s->value[size] = '\0';
  s->charset = charset;

  return s;
}

void string_resize(string** src, size_t len) {
  //printf("before string_resize %p - %lu\n", *src, len);

  size_t size = len + 1; // null terminated!

  *src = (string*) __string_reallocator(*src, sizeof(string) + size * sizeof(char));

  (*src)->capacity = size;

  //printf("after string_resize %p\n", *src);
}

string* string_clone(string* src) {
  size_t size = sizeof(string) + src->capacity * sizeof(char);

  string* out = (string*) __string_allocator(size);

  memcpy(out, src, size);

  return out;
}

string* string_clonec(const char* src, size_t len) {
  string* out = string_new(len);

  memcpy(out->value, src, len);
  out->value[len] = '\0';
  out->length = len;

  return out;
}

void string_copy(string** out, string* src) {
  //printf("string_copy %p - %p\n", *out, src);

  size_t src_len = src->length;

  if (src_len > (*out)->capacity) {
    //string_resize(out, src_len);
    string_resize(out, 50);
  }

  string* cache = *out;

  memcpy(cache->value, src->value, src_len);
  cache->length = src_len;

  cache->value[src_len] = '\0';
}

void _string_copyc(string** out, const char* src, charset_t charset) {
  //printf("string_copy %p - chars* %p\n", *out, src);

  size_t len = strlen(src);

  string* cache = *out;

  //printf("capacity check %zu - %zu\n", cache->capacity, len);
  if (len > cache->capacity) {
    string_resize(out, len);
    cache = *out;
  }
  //printf("string_copy %p @ %p\n", cache, cache->value);
  cache->length = len;
  cache->charset = charset;
  strcpy(cache->value, src);
}

void string_delete(string** str) {
  __string_deallocator((void*) *str);
  *str = __MEM_FREE_ADDR; // when free set to a know address
}

void string_clear(string* str) {
  str->value[0] = '\0';
  str->length = 0;
}

void string_cleanup() {
  if (string_def_trim_mask) {
    string_delete(&string_def_trim_mask);
  }
}
