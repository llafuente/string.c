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

int st_compare(const string* a, const string* b) {
  if (a->length > b->length) {
    return 1;
  }

  if (a->length < b->length) {
    return -1;
  }

  const char* pa = a->value, *pb = b->value;

  while (*pa && (*pa == *pb)) {
    ++pa;
    ++pb;
  }

  return *(const unsigned char*)pa - *(const unsigned char*)pb;
}

int st_cmp(const string* a, const string* b) { return st_compare(a, b); }

int st_subcompare(const string* a, const string* b, st_len_t offset,
                  st_len_t length) {

  // working range
  st__calc_range(a->length, &offset, &length);

  // length check
  if (length > b->length) {
    // printf("length [%ld] > b->length\n", diff);
    return 1;
  }

  if (length < b->length) {
    // printf("length [%ld] < b->length\n", diff);
    return -1;
  }

  char* start;
  char* end;
  st__get_char_range((string*)a, offset, length, &start, &end);

  const char* pb = b->value;

  // printf("[%c] = [%c]\n", *start, *pb);

  while (start < end && *pb && (*start == *pb)) {

    ++start;
    ++pb;
    // printf("[%c] = [%c]\n", *start, *pb);
  }
  if (!*pb) {
    return 0;
  }

  return *(const unsigned char*)start - *(const unsigned char*)pb;
}

int st_scmp(const string* a, const string* b, st_len_t offset,
            st_len_t length) {
  return st_subcompare(a, b, offset, length);
}
