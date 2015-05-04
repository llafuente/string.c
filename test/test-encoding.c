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

void test_encoding_go_back(int* utf32p, char* utf8) {
  string* src = st_newc(utf8, st_enc_utf8);

  string* go = st_to_utf32(src);
  ASSERT(wcscmp((const wchar_t*)go->value, (const wchar_t*)utf32p) == 0,
         "utf32-8 go");

  string* back = st_to_utf8(go);
  ASSERT(back != 0, "utf32-8 back is ok");

  ASSERT(strcmp(back->value, src->value) == 0, "utf32-8 back");

  st_delete(&src);
  st_delete(&go);
  st_delete(&back);
}

TASK_IMPL(encoding) {
  // greek
  test_encoding_go_back(L"ο Δικαιοπολις εν αγρω εστιν",
                        "ο Δικαιοπολις εν αγρω εστιν");
  // misc
  test_encoding_go_back(L"Iñtërnâtiônàlizætiøn", "Iñtërnâtiônàlizætiøn");

  // Jap/arab etc..
  test_encoding_go_back(L"AÀΑ╬豈Ａꊠ黠だ➀ጀะڰЯ0123456789",
                        "AÀΑ╬豈Ａꊠ黠だ➀ጀะڰЯ0123456789");

  ASSERT(st_utf8_length("123456789也不是可运行的程序１２３４５６７８９", 0) ==
             27,
         "length check");
  test_encoding_go_back(L"123456789也不是可运行的程序１２３４５６７８９",
                        "123456789也不是可运行的程序１２３４５６７８９");

  test_encoding_go_back(
      L"☃是龍𩸽𠜎 𠜱 𠝹 𠱓 𠱸 𠲖 𠳏 𠳕 𠴕 𠵼 𠵿 𠸎 𠸏 𠹷 𠺝 𠺢 𠻗 𠻹 𠻺 𠼭 𠼮 "
      L"𠽌 𠾴 𠾼 𠿪 𡁜 𡁯 𡁵 𡁶 𡁻 𡃁 𡃉 𡇙 𢃇 𢞵 𢫕 𢭃 𢯊 𢱑 𢱕 𢳂 𢴈 𢵌 𢵧 "
      L"𢺳 𣲷 𤓓 𤶸 𤷪 𥄫 𦉘 𦟌 𦧲 𦧺 𧨾 𨅝 𨈇 𨋢 𨳊 𨳍 𨳒 𩶘",
      "☃是龍𩸽𠜎 𠜱 𠝹 𠱓 𠱸 𠲖 𠳏 𠳕 𠴕 𠵼 𠵿 𠸎 𠸏 𠹷 𠺝 𠺢 𠻗 𠻹 𠻺 𠼭 𠼮 "
      "𠽌 𠾴 𠾼 𠿪 𡁜 𡁯 𡁵 𡁶 𡁻 𡃁 𡃉 𡇙 𢃇 𢞵 𢫕 𢭃 𢯊 𢱑 𢱕 𢳂 𢴈 𢵌 𢵧 𢺳 "
      "𣲷 𤓓 𤶸 𤷪 𥄫 𦉘 𦟌 𦧲 𦧺 𧨾 𨅝 𨈇 𨋢 𨳊 𨳍 𨳒 𩶘");

  return 0;
}
