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

/**
 * Converts utf8 input to utf32
 * @param    utf8[] : character encoded in utf8
 * @param    done : boolean indicating the completion of the operation (FALSE:
 * conversion not done)
 * @return   a single character encoded in UTF32
*/
string* st_to_utf32(const string* src) {
  string* out = st_new(src->length * 4, st_enc_utf32be);

  switch (src->encoding) {
  // TODO what we do?
  case st_enc_binary:
  case st_enc_ascii:
    assert(false); // TODO
  case st_enc_utf8: {

    st_uc_t* p = (st_uc_t*)src->value;
    st_uc_t* end = (st_uc_t*)p + src->used;
    st_uc4_t* dst = (st_uc4_t*)out->value;

    st_uc_t utf8;
    st_len_t len = 0;

    while (p < end) {
      utf8 = *p;
      if ((utf8 < (st_uc_t)'\200')) {
        *dst = utf8;
        ++p;
      } else if ((utf8 >= (st_uc_t)'\370')) {
        *dst = 0;
        ++p;
      } else if ((utf8 >= (st_uc_t)'\360')) {
        *dst = ((((262144 * (utf8 % 8)) + (4096 * (p[1] % 64))) +
                 (64 * (p[2] % 64))) +
                (p[3] % 64));
        p += 4;
      } else if ((utf8 >= (st_uc_t)'\340')) {
        *dst = (((4096 * (utf8 % 16)) + (64 * (p[1] % 64))) + (p[2] % 64));
        p += 3;
      } else if ((utf8 >= (st_uc_t)'\300')) {
        *dst = ((64 * (utf8 % 32)) + (p[1] % 64));
        p += 2;
      } else {
        // error
        st_delete(&out);
        return 0;
      }

      dst += 1;
    }
    *dst = '\0';
    out->used = ((char*)dst) - out->value;
    out->length = src->length;

    return out;
  }

  case st_enc_utf32le:
  case st_enc_utf32be:
    st_copy(&out, (const string*)src);
    return out;
  }
}

string* st_to_utf8(const string* src) {
  // at most it will be the same, so do it
  string* out = st_new(src->length * 4, st_enc_utf8);

  switch (src->encoding) {
  // TODO what we do?
  case st_enc_binary:
  case st_enc_ascii:
    assert(false); // TODO
  case st_enc_utf8:
    // clone
    st_copy(&out, (const string*)src);
    return out;
  case st_enc_utf32le:
  case st_enc_utf32be: {

    st_uc4_t* p = (st_uc4_t*)src->value;
    st_uc4_t* end = (st_uc4_t*)(p + src->used);
    st_uc_t* dst = (st_uc_t*)out->value;

    st_uc4_t utf32;

    while (p < end) {
      utf32 = *p;

      if (utf32 < 128) {

        dst[0] = utf32;
        ++dst;
      } else if (utf32 < 2048) {
        dst[1] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[0] = (192 + (utf32 % 32));
        dst += 2;
      } else if (utf32 < 65536) {
        dst[2] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[1] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[0] = (224 + utf32);
        dst += 3;
      } else if (utf32 < 1048576) {
        dst[3] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[2] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[1] = (128 + (utf32 % 64));
        utf32 = (utf32 * 0.015625);
        dst[0] = (240 + utf32);
        dst += 4;
      } else {
        st_delete(&out);
        return 0;
      }
      ++p;
    }
    return out;
  }
  }
}
