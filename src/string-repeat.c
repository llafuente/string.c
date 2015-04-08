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

string* string_repeat(string* src, size_t x) {
  string_len_t src_len = src->used; // @cache & use byte size not real length!
  /* Don't waste our time if it's empty */
  /* ... or if the multiplier is zero */
  if (src_len == 0 || x == 0) {
    return string_new((size_t) 0);
  }

  /* Initialize the result string */
  string_len_t result_len = src_len * x; // Length of the resulting string
  string *result = string_new(result_len, src->charset);
  // Heavy optimization for situations where src string is 1 byte long
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
  result->length = src->length * x;
  result->used = src_len * x;
  return result;
}
