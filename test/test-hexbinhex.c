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

TASK_IMPL(hexbinhex) {
  // st_bin2hex
  string* s = st_newc("1001", st_enc_ascii);
  string* aux = st_bin2hex(s);

  ASSERT_STR(aux, "31303031", st_enc_ascii);
  st_delete(&aux);

  // go
  st_copyc(&s, "1111", st_enc_ascii);
  aux = st_bin2hex(s);
  ASSERT_STR(aux, "31313131", st_enc_ascii);
  // back
  string* aux2 = st_hex2bin(aux);
  ASSERT_STR(aux2, "1111", st_enc_ascii);
  // cleanup
  st_delete(&aux);
  st_delete(&aux2);

  // once again!
  st_copyc(&s, "6e6f7420636f6d706c657465", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT_STR(aux, "not complete", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "6E6F7420636F6D706C657465", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT_STR(aux, "not complete", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "31313131", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT_STR(aux, "1111", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "6578616d706c65206865782064617461", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT_STR(aux, "example hex data", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, "6578616D706C65206865782064617461", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT_STR(aux, "example hex data", st_enc_ascii);
  st_delete(&aux);

  st_copyc(&s, T_STR_ASCII, st_enc_ascii);
  aux = st_bin2hex(s);
  aux2 = st_hex2bin(aux);

  ASSERT_STR(aux2, T_STR_ASCII, st_enc_ascii);

  st_delete(&aux);
  st_delete(&aux2);

  st_copyc(&s, "abcABCABXC", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT(aux == 0, "invalid hex");

  st_copyc(&s, "abcABCABCX", st_enc_ascii);
  aux = st_hex2bin(s);
  ASSERT(aux == 0, "invalid hex");

  st_delete(&s);

  return 0;
}
