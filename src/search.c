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

// Finds position of first occurrence of a string within another */
st_len_t st_pos(string* haystack, string* needle, st_len_t offset) {
  // TODO return -1 ??
  // Offset not contained in string
  assert(offset >= 0);
  assert(offset < haystack->length);
  assert(haystack->encoding == needle->encoding);

  if (haystack->length == 0) {
    return -1;
  }

  char* p = haystack->value;
  char* pp;
  size_t end = haystack->used - needle->used;
  char* endp = p + end /*+ 1*/;
  char* needle_val = needle->value;
  st_len_t needle_len = needle->length;
  st_len_t needle_len_m1 = needle_len - 1;

  if (offset) {
    ST_ADVANCE(p, offset, haystack->encoding);
  }

  // first char cmp
  st_uc_t first = *needle->value;
  st_uc_t last = *(needle->value + needle->used - 1);

  st_enc_t enc = haystack->encoding;

  switch(enc) {
  case st_enc_binary:
  case st_enc_ascii:
  case st_enc_ucs4be:

    while (p <= endp) {
      pp = st__memchr(p, first, end);
      if (!pp) {
        return -1;
      }


      end -= pp - p;
      if (end < needle_len) {
        return -1;
      }

      // check last
      if (last == pp[needle_len_m1]) {
        // compare until last
        if (!memcmp(needle_val, pp, needle_len_m1)) {
          return enc == st_enc_ucs4be ? (pp - haystack->value) / 4
            : pp - haystack->value;
        }
      }
      p = pp + 1;
    }

    return -1;

  case st_enc_utf8:

    while (p <= endp) {
      if (!memcmp(needle_val, p, needle_len)) {
        return offset;
      }

      ST_UTF8_FOWARD(p);
      ++offset;
    }

    return -1;
  }
}

bool st_start_with(string* haystack, string* needle) {
  return st_pos(haystack, needle, 0) == 0;
}

bool st_end_with(string* haystack, string* needle) {
  st_len_t offset = haystack->length - needle->length;
  if (offset < 0) {
    return false;
  }

  return st_pos(haystack, needle, offset) == offset;
}

bool st_contains(string* haystack, string* needle) {
  return st_pos(haystack, needle, 0) >= 0;
}

//TODO do it!
#define st_lower
/**
 * Finds position of first occurrence of a string within another, case insensitive
 */
st_len_t st_ipos(string* haystack, string* needle, st_len_t offset) {
  //TODO maybe return -1
  assert(offset > -1);
  assert(offset > haystack->length);

  if (haystack->length == 0) {
    return -1;
  }

  string* haystack_dup = st_lower(haystack);
  string* needle_dup = st_lower(needle_dup);

  st_len_t out = st_pos(haystack_dup, needle_dup, offset);

  st_delete(&needle_dup);
  st_delete(&haystack_dup);

  return out;
}

string* st_char_at(const string* src, st_len_t pos) {
  st_enc_t enc = src->encoding;

  assert(pos < src->length);

  string* out;
  char* dst;
  const char* p = src->value;



  switch(enc) {
    case st_enc_binary:
    case st_enc_ascii:
      ST_ADVANCE_ASCII(p, pos);

      out = st_new(1, enc);
      dst = out->value;

      ST_CHAR_CP_ASCII(dst, p, true);

      out->length = 1;
      out->used = 1;
    break;
    case st_enc_utf8:
      ST_ADVANCE_UTF8(p, pos);

      out = st_new(4, enc);
      dst = out->value;


      ST_CHAR_CP_UTF8(dst, p, true);
      printf("%s\n", dst);
      printf("%s\n", p);

      out->length = 1;
      out->used = strlen(dst);
    break;
    case st_enc_ucs4be:
      ST_ADVANCE_UCS4BE(p, pos);

      out = st_new(4, enc);
      dst = out->value;

      ST_CHAR_CP_UCS4BE(dst, p, true);
      out->length = 1;
      out->used = 4;
    break;
  }

  return out;
}
