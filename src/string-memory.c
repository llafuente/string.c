/*
* Copyright 2015 Luis Lafuente <llafuente@noboxout.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/// @file

#include "stringc.h"

string* _string_new(size_t len, charset_t charset) {
  size_t size = len + 1; // null terminated!

  string* s = (string*) __STRING_ALLOCATOR(sizeof(string) + size * sizeof(char));

  s->length = 0;
  s->used = 0;
  s->capacity = size;
  s->value[0] = '\0';
  s->charset = charset;

  return s;
}

//TODO use utf8_len()
string* _string_newc(const char* src, charset_t charset) {
  size_t size = strlen(src) + 1; // null terminated!

  string* s = (string*) __STRING_ALLOCATOR(sizeof(string) + (size) * sizeof(char));

  s->length = size - 1;
  s->used = size;
  s->capacity = size;
  memcpy(s->value, src, size);
  s->charset = charset;

  return s;
}

void string_resize(string** src, size_t len) {
  //printf("before string_resize %p - %lu\n", *src, len);

  size_t size = len + 1; // null terminated!

  *src = (string*) __STRING__REALLOCATOR(*src, sizeof(string) + size * sizeof(char));

  (*src)->capacity = size;

  //printf("after string_resize %p\n", *src);
}

string* string_clone(string* src) {
  size_t size = sizeof(string) + src->capacity * sizeof(char);

  string* out = (string*) __STRING_ALLOCATOR(size);

  memcpy(out, src, size);

  return out;
}

string* string_clone_subc(char* src, size_t len, charset_t charset) {
  string* out = _string_new(len + 1, charset);
  memcpy(out->value, src, len);
  out->length = len;
  out->value[len] = '\0';

  return out;
}

/* deprecate, for reference only
string* string_clonec(const char* src, size_t len) {
  string* out = string_new(len);

  memcpy(out->value, src, len);
  out->value[len] = '\0';
  out->length = len;

  return out;
}
*/

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

void _string_copyc(string** out, const char* src, charset_t enc) {
  //printf("string_copy %p - chars* %p\n", *out, src);
  STRING_GET_CHAR_DATA(src, enc, len, used);

  string* cache = *out;

  //printf("capacity check %zu - %zu\n", cache->capacity, len);
  if (len > cache->capacity) {
    string_resize(out, len);
    cache = *out;
  }
  //printf("string_copy %p @ %p\n", cache, cache->value);
  cache->length = len;
  cache->used = used;
  cache->charset = enc;
  strcpy(cache->value, src);
}

void string_delete(string** str) {
  __STRING_DEALLOCATOR((void*) *str);
  *str = __STRING_MEM_FREE_ADDR; // when free set to a know address
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
