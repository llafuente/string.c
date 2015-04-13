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

const st_uc_t st_bom[] = {0xef, 0xbb, 0xbf};

char* st__memchr(const char *s, st_uc_t c, size_t n) {
  if (n != 0) {
    const st_uc_t* p = (st_uc_t*) s;

    do {
      if (*p++ == c) {
        return ((char *)(p - 1));
      }
    } while (--n != 0);
  }

  return 0;
}

// utf8 chars to utf32 code point
st_uc4_t st__utf8c_to_utf32cp(st_uc_t* utf8) {
  st_uc_t c = *utf8;

  if ((c < (st_uc_t)'\200')) {
    return c;
  }

  if ((c >= (st_uc_t)'\370')) {
    return 0;
  }

  if ((c >= (st_uc_t)'\360')) {
    return ((((262144 * (c % 8)) + (4096 * (utf8[1] % 64))) + (64 * (utf8[2] % 64))) + (utf8[3] % 64));
  }

  if ((c >= (st_uc_t)'\340')) {
    return (((4096 * (c % 16)) + (64 * (utf8[1] % 64))) + (utf8[2] % 64));
  }

  if ((c >= (st_uc_t)'\300')) {
    return ((64 * (c % 32)) + (utf8[1] % 64));
  }

  return 0;
}

// utf32 code point to utf8 chars
st_len_t st__utf32cp_to_utf8c(st_uc4_t utf32, st_uc_t* utf8) {
  if (utf32 < 128) {
    utf8[0] = utf32;
    return 1;
  }

  if (utf32 < 2048) {
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (192 + (utf32 % 32));
    return 2;
  }

  if (utf32 < 65536) {
    utf8[2] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (224 + utf32);
    return 3;
  }

  if (utf32 < 1048576) {
    utf8[3] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[2] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[1] = (128 + (utf32 % 64));
    utf32 = (utf32 / 64);
    utf8[0] = (240 + utf32);
    return 4;
  }
  return 0; //err
}
