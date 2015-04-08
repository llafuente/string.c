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

string* string_def_trim_mask = 0;

// TODO review, cache charmask for !what_str
string* string_trim(string *str, string *what_str, int mode) {
  const char *c = str->value;
  size_t len = str->length;
  size_t i;
  size_t trimmed = 0;

  if (!what_str) {
    if (!string_def_trim_mask) {
      string_def_trim_mask = string_newc(" \t\n\r\0\x0B", string_enc_ascii);
    }
    what_str = string_def_trim_mask;
  }
  const char *what = what_str->value;
  size_t what_len = what_str->length;

  if (what_len == 1) {
    char whatc = *what;
    if (mode & 1) {
      for (i = 0; i < len; i++) {
        if (c[i] == whatc) {
          trimmed++;
        } else {
          break;
        }
      }
      len -= trimmed;
      c += trimmed;
    }
    if (mode & 2) {
      if (len > 0) {
        i = len - 1;
        do {
          if (c[i] == whatc) {
            len--;
          } else {
            break;
          }
        } while (i-- != 0);
      }
    }
  } else {
    char mask[256];
    string_charmask((unsigned char*)what, what_len, mask);
    if (mode & 1) {
      for (i = 0; i < len; i++) {
        if (mask[(unsigned char)c[i]]) {
          trimmed++;
        } else {
          break;
        }
      }
      len -= trimmed;
      c += trimmed;
    }
    if (mode & 2) {
      if (len > 0) {
        i = len - 1;
        do {
          if (mask[(unsigned char)c[i]]) {
            len--;
          } else {
            break;
          }
        } while (i-- != 0);
      }
    }
  }

  if (str->length == len) {
    return string_clone(str);
  }

  return string_clone_subc(c, len, str->encoding);
}
