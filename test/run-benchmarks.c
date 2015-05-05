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

#include "tasks.h"
#include "fixtures.h"

int main(int argc, const char* argv[]) {

  printf("#########################\n");
  printf("## string.c benchmarks ##\n");
  printf("#########################\n");

  if (argc == 1) {
    printf("Usage: benchmarks target-file");
    exit(1);
  }

  const char* prefix = argv[1];

  TASK_BENCHMARK(prefix, 25, 500, alloc);
  TASK_BENCHMARK(prefix, 25, 500, append);
  TASK_BENCHMARK(prefix, 25, 500, ascii);
  TASK_BENCHMARK(prefix, 25, 500, case);
  TASK_BENCHMARK(prefix, 25, 500, compare);
  TASK_BENCHMARK(prefix, 25, 500, encoding);
  TASK_BENCHMARK(prefix, 25, 500, from);
  TASK_BENCHMARK(prefix, 25, 500, hexbinhex);
  TASK_BENCHMARK(prefix, 25, 500, internal);
  TASK_BENCHMARK(prefix, 25, 500, iterators);
  TASK_BENCHMARK(prefix, 25, 500, justify);
  TASK_BENCHMARK(prefix, 25, 500, repeat);
  TASK_BENCHMARK(prefix, 25, 500, search);
  TASK_BENCHMARK(prefix, 25, 500, shuffle);
  TASK_BENCHMARK(prefix, 25, 500, sub);
  TASK_BENCHMARK(prefix, 25, 500, to);
  TASK_BENCHMARK(prefix, 25, 500, trim);
  TASK_BENCHMARK(prefix, 25, 500, utf32);
  TASK_BENCHMARK(prefix, 25, 500, utf8);
  TASK_BENCHMARK(prefix, 25, 500, utils);

  st_cleanup();
  printf("OK\n");

  return 0;
}
