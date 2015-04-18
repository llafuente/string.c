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
// TODO add st_len_t len (to search always inside a substring)
// Finds position of first occurrence of a string within another */
st_len_t st_pos(const string* haystack, const string* needle, st_len_t offset,
                st_len_t length) {
  // caches
  st_len_t nlen = needle->length;

  // working range
  st__calc_range(haystack->length, &offset, &length);
  length -= nlen;

  // needle greater than substr?
  if (length < 0) {
    return -1;
  }

  char* start;
  char* end;
  st__get_char_range(haystack, offset, length, &start, &end);

  // first char to comare
  const char* needle_val = needle->value;
  st_len_t needle_len = needle->length;
  st_len_t needle_len_m1 = needle_len - 1;
  char* pp;
  st_uc_t first = *needle_val;
  // last char to comare
  st_uc_t last = *(needle_val + needle->used - 1);

  st_enc_t enc = haystack->encoding;

  st_len_t end_len;
  switch (enc) {
  case st_enc_binary:
  case st_enc_ascii:
  case st_enc_utf32le:
  case st_enc_utf32be:

    end_len = end - start;

    while (start <= end) {
      pp = st__memchr(start, first, end - start);
      if (!pp) {
        return -1;
      }

      end_len -= pp - start;
      if (end_len < needle_len) {
        return -1;
      }

      // check last
      if (last == pp[needle_len_m1]) {
        // compare until last
        if (!memcmp(needle_val, pp, needle_len_m1)) {
          return (enc == st_enc_utf32be) ? (pp - haystack->value) * 0.25 // /4
                                         : pp - haystack->value;
        }
      }
      start = pp + 1;
    }

    return -1;

  case st_enc_utf8:

    while (start <= end) {
      if (!memcmp(needle_val, start, needle_len)) {
        return offset;
      }

      ST_UTF8_FOWARD(start);
      ++offset;
    }

    return -1;
  }
}

bool st_start_with(const string* haystack, const string* needle) {
  return st_pos(haystack, needle, 0, 0) == 0;
}

bool st_end_with(const string* haystack, const string* needle) {
  st_len_t offset = haystack->length - needle->length;
  if (offset < 0) {
    return false;
  }

  return st_pos(haystack, needle, offset, 0) == offset;
}

bool st_contains(const string* haystack, const string* needle) {
  return st_pos(haystack, needle, 0, 0) >= 0;
}

st_len_t st_ipos(const string* haystack, const string* needle, st_len_t offset,
                 st_len_t length) {

  if (haystack->length == 0) {
    return -1;
  }

  string* haystack_dup = st_lower(haystack);
  string* needle_dup = st_lower(needle);

  st_len_t out = st_pos(haystack_dup, needle_dup, offset, length);

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

  switch (enc) {
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

    out->length = 1;
    out->used = strlen(dst);
    break;
  case st_enc_utf32le:
  case st_enc_utf32be:
    ST_ADVANCE_UTF32(p, pos);

    out = st_new(4, enc);
    dst = out->value;

    ST_CHAR_CP_UTF32(dst, p, true);
    out->length = 1;
    out->used = 4;
    break;
  }

  return out;
}

st_len_t st_rpos(string* haystack, string* needle, st_len_t offset,
                 st_len_t len) {
  assert(haystack->encoding == needle->encoding);

  // caches
  st_len_t nlen = needle->length;

  // working range
  st_len_t start_pos = offset;
  st_len_t end_pos = len;
  st__calc_range(haystack->length, &start_pos, &end_pos);
  end_pos -= nlen;

  char* start;
  char* end;
  st__get_char_range(haystack, start_pos, end_pos, &start, &end);

  // printf("start [%p] end [%p] \n", start, end);
  // printf("diff [%ld] \n", end - start);

  size_t bytes = needle->used;

  char* nval = needle->value;

  // printf("start [%p] end2 [%p] \n", start, end);
  // printf("diff [%ld] \n", end - start);

  // loop backwards
  switch (haystack->encoding) {
  case st_enc_binary:
  case st_enc_ascii:
    while (start <= end) {
      if (!memcmp(nval, end, bytes)) {
        return end_pos;
      }

      --end;
      --end_pos;
    }

    return -1;
  case st_enc_utf8:

    while (start <= end) {
      if (!memcmp(nval, end, bytes)) {
        return end_pos;
      }

      ST_UTF8_BACK(end);
      --end_pos;
    }

    return -1;
  case st_enc_utf32le:
  case st_enc_utf32be:
    while (start <= end) {
      if (!memcmp(nval, end, bytes)) {
        return end_pos;
      }

      end -= 4;
      --end_pos;
    }

    return -1;
  }
}

st_len_t st_irpos(const string* haystack, const string* needle, st_len_t offset,
                  st_len_t length) {

  if (haystack->length == 0) {
    return -1;
  }

  string* haystack_dup = st_lower(haystack);
  string* needle_dup = st_lower(needle);

  st_len_t out = st_rpos(haystack_dup, needle_dup, offset, length);

  st_delete(&needle_dup);
  st_delete(&haystack_dup);

  return out;
}

string* st_remove(const string* haystack, const string* needle, st_len_t offset,
                  st_len_t length) {
  // caches
  st_len_t nlen = needle->length;

  // working range
  st__calc_range(haystack->length, &offset, &length);

  // caches
  st_len_t nused = needle->used;
  const char* nval = needle->value;
  const char* hval = haystack->value;
  st_enc_t enc = haystack->encoding;

  char* start;
  char* end;
  st__get_char_range(haystack, offset, length, &start, &end);

  string* out = st_new(haystack->used, enc);

  st_len_t last_hay_pos = start - hval;
  st_len_t last_out_pos = 0;
  st_len_t chars_to_cpy = 0;
  st_len_t need_cpy = 0;

  st_len_t olen = 0;
  st_len_t oused = 0;
  char* oval = out->value;

  printf("last_hay_pos %ld\n", last_hay_pos);
  printf("last_out_pos %ld\n", last_out_pos);
  printf("need_cpy %ld\n", need_cpy);

  if (last_hay_pos) {
    memcpy(oval, hval, last_hay_pos);
    last_out_pos = last_hay_pos;
    olen = offset;
    oused = last_hay_pos;
  }

  // first char to comare

  st_len_t needle_len_m1 = nused - 1;
  char* pp;
  st_uc_t first = *nval;
  // last char to comare
  st_uc_t last = *(nval + nused - 1);

  st_len_t end_len;

  st_len_t jump;
  st_len_t itr = 0;

  // printf("end - start = %ld\n", end - start);

  while (start < end) {
    // printf("****** itr %ld [%c]\n", itr, *start);
    ++itr;

    // printf("last_hay_pos %ld\n", last_hay_pos);
    // printf("last_out_pos %ld\n", last_out_pos);
    // printf("need_cpy %ld\n", need_cpy);
    // printf("chars_to_cpy %ld\n", chars_to_cpy);

    if (!memcmp(nval, start, nused)) {
      // cpy and update
      // printf("*- CPY!\n");
      if (need_cpy) {
        olen += chars_to_cpy;
        oused += need_cpy;
        memcpy(oval + last_out_pos, hval + last_hay_pos, need_cpy);
        chars_to_cpy = 0;

        last_out_pos += need_cpy;
        last_hay_pos += need_cpy + nused;
        need_cpy = 0;
      } else {
        last_hay_pos += nused;
      }
      start += nused;
      continue;
    }

    jump = st_char_size(start, enc);
    ++chars_to_cpy;
    start += jump;
    need_cpy += jump;
  }

  // printf("*last_hay_pos %ld\n", last_hay_pos);
  // printf("*last_out_pos %ld\n", last_out_pos);
  // printf("*chars_to_cpy %ld\n", chars_to_cpy);
  // printf("*need_cpy %ld\n", need_cpy);

  out->length = olen + chars_to_cpy;
  out->used = oused + need_cpy;
  memcpy(oval + last_out_pos, hval + last_hay_pos, need_cpy);
  last_out_pos += need_cpy;

  *(out->value + last_out_pos) = '\0';
  if (enc == st_enc_utf32be || enc == st_enc_utf32le) {
    *(out->value + last_out_pos + 1) = '\0';
    *(out->value + last_out_pos + 2) = '\0';
    *(out->value + last_out_pos + 3) = '\0';
  }
  return out;
}
