#include "stringc.h"

static const char string_utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};
const char * const string_utf8_skip = string_utf8_skip_data;

/**
* return length of given characters supossing is UTF-8
* @credits based on glib_utf8_offset_to_pointer
*/
size_t string_utf8_lenc(const char* c) {
  size_t len = 0;
  const char* p = c;
  while (*p != '\0') {
    //printf("%c @%p %d\n", *p, p, utf8_next(p));
    p += string_utf8_jump_next(p);
    //printf("%c\n", *p);
    ++len;
  }

  return len;
}

/*
* is_utf8 is distributed under the following terms:
* Copyright (c) 2013 Palard Julien. All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/**
* from: https://github.com/JulienPalard/is_utf8/
*
* Check if the given unsigned char * is a valid utf-8 sequence.
* Return value :
* If the string is valid utf-8, 0 is returned.
* Else the position, starting from 1, is returned.
* Valid utf-8 sequences look like this :
* 0xxxxxxx
* 110xxxxx 10xxxxxx
* 1110xxxx 10xxxxxx 10xxxxxx
* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
*
*/
int string_is_utf8(unsigned char *str, size_t len, size_t* first_invalid_pos) {
  size_t i = 0;
  size_t j = 0;
  size_t continuation_bytes = 0;
  unsigned char cache;

  while (i < len) {
    j = i;
    cache = str[i];

    if (cache <= 0x7F) {
      continuation_bytes = 0;
    } else if (cache >= 0xC0 /*11000000*/ && cache <= 0xDF /*11011111*/) {
      continuation_bytes = 1;
    } else if (cache >= 0xE0 /*11100000*/ && cache <= 0xEF /*11101111*/) {
      continuation_bytes = 2;
    } else if (cache >= 0xF0 /*11110000*/ && cache <= 0xF4 /* Cause of RFC 3629 */) {
      continuation_bytes = 3;
    } else {
      if (first_invalid_pos) {
        *first_invalid_pos = j;
      }
      return i + 1;
    }

    i += 1;
    while (i < len && continuation_bytes > 0
      && cache >= 0x80
      && cache <= 0xBF) {
      i += 1;
      continuation_bytes -= 1;
    }
    if (continuation_bytes != 0) {
      if (first_invalid_pos) {
        *first_invalid_pos = j;
      }
      return i + 1;
    }
  }

  return 0;
}
