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

string* st_new(size_t cap, st_enc_t enc) {
  size_t size = cap + 1; // null terminated!

  string* s = (string*) __STRING_ALLOCATOR(sizeof(string) + size * sizeof(char));

  s->length = 0;
  s->used = 0;
  s->capacity = size;
  s->value[0] = '\0';
  s->encoding = enc;

  return s;
}

//TODO use utf8_len()
string* st_newc(const char* src, st_enc_t enc) {
  STRING_GET_CHAR_DATA(src, enc, len, used);
  size_t size = used + 1; // null terminated!

//printf("%s\n",  src);
//printf("len[%zu] used[%zu] enc[%d]", len, used, enc);

  string* s = (string*) __STRING_ALLOCATOR(sizeof(string) + (size) * sizeof(char));

  s->length = len;
  s->used = used;
  s->capacity = size;
  memcpy(s->value, src, size);
  s->encoding = enc;

  return s;
}

void st_resize(string** src, size_t cap) {
  //printf("before st_resize %p - %lu\n", *src, cap);

  size_t size = cap + 1; // null terminated!

  *src = (string*) __STRING__REALLOCATOR(*src, sizeof(string) + size * sizeof(char));

  (*src)->capacity = size;

  //printf("after st_resize %p\n", *src);
}

string* st_clone(string* src) {
  size_t size = sizeof(string) + src->capacity * sizeof(char);

  string* out = (string*) __STRING_ALLOCATOR(size);

  memcpy(out, src, size);

  return out;
}

string* st_clone_subc(char* src, size_t len, st_enc_t enc) {
  assert(enc == st_enc_ascii);

  string* out = st_new(len, enc);
  memcpy(out->value, src, len);
  out->length = len;
  out->used = len;
  out->value[len] = '\0';

  return out;
}

/* deprecate, for reference only
string* st_clonec(const char* src, size_t len) {
  string* out = st_new(len);

  memcpy(out->value, src, len);
  out->value[len] = '\0';
  out->length = len;

  return out;
}
*/

void st_copy(string** out, string* src) {
  //printf("st_copy %p - %p\n", *out, src);

  size_t src_len = src->length;

  if (src_len > (*out)->capacity) {
    //st_resize(out, src_len);
    st_resize(out, 50);
  }

  string* cache = *out;

  memcpy(cache->value, src->value, src_len);
  cache->length = src_len;
  cache->used = src->used;

  cache->value[src_len] = '\0';
}

void st_copyc(string** out, const char* src, st_enc_t enc) {
  //printf("st_copy %p - chars* %p\n", *out, src);
  STRING_GET_CHAR_DATA(src, enc, len, used);

  string* cache = *out;

  //printf("capacity check %zu - %zu\n", cache->capacity, len);
  if (len > cache->capacity) {
    st_resize(out, len);
    cache = *out;
  }
  //printf("st_copy %p @ %p\n", cache, cache->value);
  cache->length = len;
  cache->used = used;
  cache->encoding = enc;
  strcpy(cache->value, src);
}

void st_delete(string** out) {
  __STRING_DEALLOCATOR((void*) *out);
  *out = __STRING_MEM_FREE_ADDR; // when free set to a know address
}

void st_clear(string* out) {
  out->value[0] = '\0';
  out->length = 0;
  out->used = 0;
}

void st_cleanup() {
  if (string_def_trim_mask) {
    st_delete(&string_def_trim_mask);
  }
}
