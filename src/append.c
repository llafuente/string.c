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

// TODO test & handle diff/enc
void st_append(string** out, string* src) {
  // printf("string_append %p - %p\n", *out, src);

  string* cache = *out;

  st_len_t bytes_to_cpy = src->used;
  size_t cap = cache->capacity;

  if (cap < cache->used + bytes_to_cpy) {
    // out will be reallocated if src is the same we should use it.
    bool same = cache == src;

    // assert(*out == src);
    st_resize(out, cache->used + bytes_to_cpy);
    cache = *out;
    if (same) {
      src = cache;
    }
  }

  memcpy(cache->value + cache->used, src->value, bytes_to_cpy);
  cache->length += src->length;
  cache->used += bytes_to_cpy;

  cache->value[cache->used] = '\0';
}

void st_append_char(string** out, st_uc_t ch) {
  st_enc_t enc = (*out)->encoding;
  assert(enc == st_enc_ascii || enc == st_enc_utf8);

  st_grow(out, (*out)->used + 1, enc);
  string* cache = *out;

  cache->value[cache->used] = ch;
  ++cache->length;
  ++cache->used;
  cache->value[cache->used] = '\0'; // can be asumed beacause st_uc_t
}

string* st_concat(string* first, string* second) {
  assert(first->encoding == second->encoding);

  size_t f_used = first->used;
  size_t s_used = second->used;
  string* out = st_new(f_used + s_used, first->encoding);
  char* dst = out->value;
  memcpy(dst, first->value, f_used);
  memcpy(dst + f_used, second->value, s_used);

  out->used = f_used + s_used;
  out->length = first->length + second->length;

  dst[out->used] = '\0';

  return out;
}

void st__random_ascii(char* s, const int len) {
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  int i;
  for (i = 0; i < len; ++i) {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  s[len] = 0;
}

string* st_concat_random(string* first, st_len_t len) {
  string* second = st_new(len, st_enc_ascii);
  st__random_ascii(second->value, len);
  second->used = len;
  second->length = len;

  size_t f_used = first->used;
  size_t s_used = second->used;
  string* out = st_new(f_used + s_used, first->encoding);
  char* dst = out->value;
  memcpy(dst, first->value, f_used);
  memcpy(dst + f_used, second->value, s_used);

  out->used = f_used + s_used;
  out->length = first->length + second->length;

  dst[out->used] = '\0';

  st_delete(&second);

  return out;
}
