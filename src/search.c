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

st_len_t st_index_of(const string* haystack, const string* needle,
                     st_len_t offset, st_len_t length) {
  return st_pos(haystack, needle, offset, length);
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

  size_t bytes = needle->used;

  char* nval = needle->value;

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
  st_len_t hused = haystack->used;
  const char* nval = needle->value;
  const char* hval = haystack->value;
  st_enc_t enc = haystack->encoding;

  char* start;
  char* end;
  st__get_char_range(haystack, offset, length, &start, &end);

  string* out = st_new(hused, enc);

  st_len_t last_hay_pos = start - hval;
  st_len_t last_out_pos = 0;
  st_len_t chars_to_cpy = 0;
  st_len_t need_cpy = 0;

  st_len_t olen = 0;
  st_len_t oused = 0;
  char* oval = out->value;

  if (last_hay_pos) {
    memcpy(oval, hval, last_hay_pos);
    last_out_pos = last_hay_pos;
    olen = offset;
    oused = last_hay_pos;
  }

  // first char to comare

  st_len_t needle_len_m1 = nused - 1;
  char* pp;
  st_len_t end_len;
  st_len_t jump;

  end -= nused; // avoid memcmp overflow
  while (start <= end) {
    if (!memcmp(nval, start, nused)) {
      // cpy and update
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

  end = (char*)hval + hused; // now reach the real end
  while (start < end) {
    jump = st_char_size(start, enc);
    ++chars_to_cpy;
    start += jump;
    need_cpy += jump;
  }

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

string* st_replace(const string* haystack, const string* needle,
                   const string* replacement, st_len_t* count) {
  st_enc_t enc = haystack->encoding;
  st_len_t hused = haystack->used;
  st_len_t replacements = 0;
  if (!hused) { // no haystack, do nothing!
    return st_new(0, enc);
  }

  string* out = st_new_max(hused, enc);

  st_len_t nused = needle->used;
  st_len_t rused = replacement->used;
  const char* nval = needle->value;
  const char* hval = haystack->value;
  const char* rval = replacement->value;
  char* oval = out->value;
  char* ofval = oval; // cache first

  const char* start = hval;
  const char* end = hval + hused;
  st_len_t need_cpy = 0;
  st_len_t jump;
  st_len_t olen = 0;
  st_len_t rlen = replacement->length;

  st_len_t oval_pos;

  while (start < end) {
    if (!memcmp(nval, start, nused)) {
      oval_pos = (oval - ofval);

      // this formula is very conservative, maybe we can increase it
      // TODO performance check
      st_grow(&out, oval_pos + need_cpy + rused + rused + (end - start), enc);
      // redo caches
      ofval = out->value;
      oval = ofval + oval_pos;

      ++replacements;
      // needle found -> replace
      if (need_cpy) {
        // copy previous
        memcpy(oval, start - need_cpy, need_cpy);
        oval += need_cpy;
        need_cpy = 0;
      }

      // copy replacement and advance
      memcpy(oval, rval, rused);
      oval += rused;

      olen += rlen;
      start += nused;
      continue;
    }
    // next char

    jump = st_char_size(start, enc);
    ++olen;
    start += jump;
    need_cpy += jump;
  }

  if (need_cpy) {
    st_grow(&out, (oval - ofval) + need_cpy, enc);
    // copy the rest
    memcpy(oval, start - need_cpy, need_cpy);
    oval += need_cpy;
    need_cpy = 0;
  }
  // zeronull, length and usage
  st__zeronull(oval, 0, enc);

  out->length = olen;
  out->used = oval - out->value;

  if (count) {
    *count = replacements;
  }

  return out;
}

// TODO benchmark
st_len_t st_span(const string* subject, const string* mask, st_len_t offset,
                 st_len_t length, bool contains) {
  st_len_t mlen = mask->length;

  if (mlen == 0) {
    return 0;
  }

  // caches
  st_len_t slen = subject->length;
  st_enc_t enc = subject->encoding;

  // working range
  st__calc_range(slen, &offset, &length);

  char* start;
  char* end;
  st__get_char_range(subject, offset, length, &start, &end);
  st_len_t jump;
  st_len_t mjump;
  char* mval = (char*)mask->value;
  char* mend = mval + mask->used;
  st_uc_t* mp;
  st_len_t pos = offset;

  // chararacters caches
  st_uc_t sc0;
  st_uc_t sc1;
  st_uc_t sc2;
  st_uc_t sc3;
  bool f;

  while (start <= end) {
    jump = st_char_size(start, enc);
    f = contains;

    switch (jump) {
    case 1:
      sc0 = (st_uc_t)start[0];
      break;
    case 2:
      sc0 = (st_uc_t)start[0];
      sc1 = (st_uc_t)start[1];
      break;
    case 3:
      sc0 = (st_uc_t)start[0];
      sc1 = (st_uc_t)start[1];
      sc2 = (st_uc_t)start[2];
      break;
    case 4:
      sc0 = (st_uc_t)start[0];
      sc1 = (st_uc_t)start[1];
      sc2 = (st_uc_t)start[2];
      sc3 = (st_uc_t)start[3];
      break;
    }

    mp = (st_uc_t*)mval;
    while (mp < (st_uc_t*)mend) {
      mjump = st_char_size((char*)mp, enc);
      if ((jump != mjump) || (sc0 != mp[0]) || (jump > 1 && sc1 != mp[1]) ||
          (jump > 2 && sc2 != mp[2]) || (jump > 3 && sc3 != mp[3])) {
        mp += mjump;
        continue;
      }
      f = !f;
      break;
    }

    if (!f) {
      return pos;
    }

    ++pos;
    start += jump;
  }

  return pos;
}

st_len_t st_spn(const string* subject, const string* mask, st_len_t offset,
                st_len_t length) {
  return st_span(subject, mask, offset, length, false);
}
st_len_t st_cspn(const string* subject, const string* mask, st_len_t offset,
                 st_len_t length) {
  return st_span(subject, mask, offset, length, true);
}

void st_insert(string** out, const string* ins, st_len_t offset) {
  // caches
  // check capacity -> resize
  string* o = (*out);
  st_len_t iused = ins->used;
  st_len_t oused = o->used;
  st_len_t rused = oused + iused;
  st_enc_t enc = o->encoding;
  st_grow(out, rused, enc);
  // re-cache, maybe it was reallocated
  o = (*out);

  // working range
  offset = st__calc_offset(o->length, offset);

  char* oval = o->value;
  char* p = oval;

  if (offset) {
    ST_ADVANCE(p, offset, enc);
  }

  // move from offset to the end of ins
  st_len_t left = oused - (p - oval);
  memmove(p + iused, p, left);
  // move ins to offset
  memmove(p, ins->value, iused);
  // zero null the end
  st__zeronull(oval + rused, 0, enc);
  o->used = rused;
  o->length += ins->length;
}
