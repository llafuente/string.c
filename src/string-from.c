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
* bindec, 2
* octdec, 8
* hexdec, 16
*/
double string_from_base(string *src, int base) {
  size_t num = 0;
  double fnum = 0;
  size_t i;
  int mode = 0;
  char c, *s = src->value;
  size_t cutoff;
  int cutlim;

  cutoff = INT64_MAX / base;
  cutlim = INT64_MAX % base;
  for (i = src->length; i > 0; i--) {
    c = *s++;
    /* might not work for EBCDIC */
    if (c >= '0' && c <= '9') {
      c -= '0';
    } else if (c >= 'A' && c <= 'Z') {
      c -= 'A' - 10;
    } else if (c >= 'a' && c <= 'z') {
      c -= 'a' - 10;
    } else {
      continue;
    }

    if (c >= base) {
      continue;
    }

    switch (mode) {
      case 0: /* Integer */
      if (num < cutoff || (num == cutoff && c <= cutlim)) {
        num = num * base + c;
        break;
      } else {
        fnum = (double)num;
        mode = 1;
      }
      /* fall-through */
      case 1: /* Float */
      fnum = fnum * base + c;
    }
  }

  if (mode == 1) {
    return fnum;
  }

  return num;
}


/*
* Convert a long to a string containing a base(2-36) representation of
* the number.
* decbin, 2
* decoct, 8
* dechex, 16
*/
string* string_from_number(size_t value, int base) {
  static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  string* result;
  char *ptr;
  char *end;
  char buf[(sizeof(double) << 3) + 1]; // maximum size for binary

  end = ptr = buf + sizeof(buf) - 1;
  *ptr = '\0';
  // reverse buffer filling data, ptr will be char* containing data
  do {
    *--ptr = digits[value % base];
    value /= base;
  } while (ptr > buf && value);
  //?? *end = '\0';
  // size
  result = string_new(end - ptr, string_enc_ascii);
  string_copyc(&result, ptr, string_enc_ascii);
  //result->length = end - ptr;
  return result;
}
