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

#include "stringc.h"
#include "fixtures.h"
#include "tasks.h"

int call_count = 0;

void* my_malloc(size_t bytes) {
  ++call_count;
  return malloc(bytes);
}

void* my_realloc(void* ptr, size_t bytes) {
  ++call_count;
  return realloc(ptr, bytes);
}

void my_free(void* ptr) {
  ++call_count;
  free(ptr);
}

TASK_IMPL(customalloc) {

  st_replace_allocators(my_malloc, my_realloc, my_free);

  string* s = st_newc(T_STR_CMP1, st_enc_ascii);

  ASSERT(call_count == 1, "one malloc");

  st_copyc(&s, T_STR_CMP3 T_STR_CMP3 T_STR_CMP3, st_enc_ascii);

  ASSERT(call_count == 2, "one realloc");

  st_delete(&s);
  ASSERT(call_count == 3, "one free");

  st_replace_allocators(malloc, realloc, free);

  return 0;
}
