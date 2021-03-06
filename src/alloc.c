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

string* st_new(st_size_t cap, st_enc_t enc) {
  st_size_t size = cap + st__zeronull_size(enc);

  string* s = (string*)st__malloc(sizeof(string) + size * sizeof(char));

  s->length = 0;
  s->used = 0;
  s->capacity = size;
  s->encoding = enc;

  st__zeronull(s->value, 0, enc);

  return s;
}

string* st_new_max(st_len_t len, st_enc_t enc) {
  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii:
    return st_new(len, enc);
  case st_enc_utf8:
  case st_enc_utf32le:
  case st_enc_utf32be:
    return st_new(len * 4, enc);
  }
}

string* st_newc(const char* src, st_enc_t enc) {
  st_len_t len;
  st_size_t used;

  st_get_meta(src, enc, &len, &used);
  st_size_t size = used + st__zeronull_size(enc);

  string* s = (string*)st__malloc(sizeof(string) + (size) * sizeof(char));

  s->length = len;
  s->used = used;
  s->capacity = size;
  memcpy(s->value, src, size); // copy null
  s->encoding = enc;

  return s;
}

void st_resize(string** src, st_size_t cap) {
  // printf("before st_resize %p - %lu\n", *src, cap);

  st_size_t size =
      cap + st__zeronull_size((*src)->encoding); // null terminated!

  *src = (string*)st__realloc(*src, sizeof(string) + size * sizeof(char));

  (*src)->capacity = size;

  // printf("after st_resize %p\n", *src);
}

void st_grow(string** src, st_size_t cap, st_enc_t enc) {
  if (*src == 0) {
    *src = st_new(cap, enc);
    return;
  }

  if (cap + st__zeronull_size(enc) > (*src)->capacity) {
    (*src)->encoding = enc;
    st_resize(src, cap);
  }
}

string* st_clone(const string* src) {
  size_t size = sizeof(string) + src->capacity * sizeof(char);

  string* out = (string*)st__malloc(size);

  memcpy(out, src, size); // copy everything

  return out;
}

// resize & clone
string* st_rclone(const string* src, st_size_t cap) {
  assert(cap >= src->capacity);

  cap += st__zeronull_size(src->encoding);

  size_t size = sizeof(string) + cap * sizeof(char);
  size_t src_size = sizeof(string) + src->capacity * sizeof(char);

  string* out = (string*)st__malloc(size);

  memcpy(out, src, src_size); // copy everything
  out->capacity = cap;

  return out;
}

string* st_new_subc(const char* src, st_size_t bytes, st_enc_t enc) {
  string* out = st_new(bytes, enc);
  char* oval = out->value;

  memcpy(oval, src, bytes);

  st__zeronull(oval, bytes, enc);

  out->length = st_length(oval, enc);
  out->used = bytes;

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

void st_copy(string** out, const string* src) {
  // printf("st_copy %p - %p\n", *out, src);

  size_t src_used = src->used;
  string* cache = *out;
  st_enc_t enc = src->encoding;

  if (src_used > cache->capacity) {
    st_resize(out, src_used);
    cache = *out;
  }

  char* cval = cache->value;

  memcpy(cval, src->value, src_used + st__zeronull_size(enc));
  cache->length = src->length;
  cache->encoding = enc;
  cache->used = src_used;
}

void st_copyc(string** out, const char* src, st_enc_t enc) {
  // printf("st_copy %p - chars* %p\n", *out, src);
  st_len_t len;
  st_size_t used;

  st_get_meta(src, enc, &len, &used);

  string* cache = *out;

  // printf("capacity check %zu - %zu\n", cache->capacity, len);
  if (used > cache->capacity) {
    st_resize(out, used);
    cache = *out;
  }
  // printf("st_copy %p @ %p\n", cache, cache->value);
  cache->length = len;
  cache->used = used;
  cache->encoding = enc;
  memcpy(cache->value, src, used + st__zeronull_size(enc));
}

void st_delete(string** out) {
  st__free((void*)*out);
  *out = __STRING_MEM_FREE_ADDR; // when free set to a know address
}

void st_clear(string* out) {
  st__zeronull(out->value, 0, out->encoding);
  out->length = 0;
  out->used = 0;
}

st_size_t st_dump_header_size() {
  return (sizeof(st_len_t) + sizeof(st_len_t) + sizeof(st_size_t) +
          sizeof(st_enc_t));
}

void st_dump_header(string* s, char* buff) {
  size_t slen = sizeof(string);
  char* p = (char*)s;
  char c[3];

  char* pp = buff;

  do {
    // printf("len [%lu] char[%c | %2.2x]\n", slen, *p, (*p) & 0xff);
    sprintf(c, "%2.2x", (*p) & 0xff);
    pp[0] = '\\';
    pp[1] = 'x';
    pp[2] = c[0];
    pp[3] = c[1];

    ++p;
    pp += 4;
  } while (--slen);
}

char* st_dump(string* s) {
  size_t slen = st_dump_header_size();
  size_t len = s->used;

  char* out = malloc((4 * slen) + len + 1);
  char* p = s->value; // dump values on by one
  char* pp = out + slen * 4;

  st_dump_header(s, out);

  do {
    *pp = *p;
    ++p;
    ++pp;
  } while (--len);

  *pp = '\0';
  return out;
}
