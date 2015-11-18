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

  printf("    ########################\n");
  printf("    ## string.c unit test ##\n");
  printf("    ########################\n");

  TASK_RUN(alloc);
  TASK_RUN(append);
  TASK_RUN(ascii);
  TASK_RUN(case);
  TASK_RUN(compare);
  TASK_RUN(customalloc);
  TASK_RUN(encoding);
  TASK_RUN(from);
  TASK_RUN(hexbinhex);
  TASK_RUN(internal);
  TASK_RUN(iterators);
  TASK_RUN(justify);
  TASK_RUN(repeat);
  TASK_RUN(search);
  TASK_RUN(shuffle);
  TASK_RUN(sub);
  TASK_RUN(to);
  TASK_RUN(trim);
  TASK_RUN(utf32);
  TASK_RUN(utf8);
  TASK_RUN(utils);

  st_memfree();
  printf("OK\n");

  return 0;
}
