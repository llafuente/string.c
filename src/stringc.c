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

st_malloc_func st__replaced_malloc = 0;
st_free_func st__replaced_free = 0;
st_realloc_func st__replaced_realloc = 0;

void* st__malloc(size_t size) {
  if (st__replaced_malloc)
    return (*st__replaced_malloc)(size);
  return malloc(size);
}

void st__free(void* ptr) {
  if (st__replaced_free)
    (*st__replaced_free)(ptr);
  else
    free(ptr);
}

void* st__realloc(void* ptr, size_t size) {
  if (st__replaced_realloc)
    return (*st__replaced_realloc)(ptr, size);

  return realloc(ptr, size);
}

void st_replace_allocators(st_malloc_func malloc_func,
                           st_realloc_func realloc_func,
                           st_free_func free_func) {
  if (malloc_func) {
    st__replaced_malloc = malloc_func;
  }

  if (realloc_func) {
    st__replaced_realloc = realloc_func;
  }

  if (free_func) {
    st__replaced_free = free_func;
  }
}

void st_memfree() {
  if (string_def_trim_mask) {
    st_delete(&string_def_trim_mask);
  }
}
