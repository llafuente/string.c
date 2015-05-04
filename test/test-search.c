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

TASK_IMPL(search) {
  string* replacement;
  string* hay = st_newc("0123456789", st_enc_ascii);
  string* ned = st_newc("5", st_enc_ascii);

  st_len_t i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 5, "st_pos = 5");

  i = st_pos(hay, ned, 0, 3);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("0X123456XX789", st_enc_ascii);
  ned = st_newc("XX", st_enc_ascii);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 8, "st_pos = 8");

  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("ababababdababdababddx", st_enc_ascii);
  ned = st_newc("d", st_enc_ascii);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 8, "st_pos = 8");

  i = st_pos(hay, ned, 9, 0);
  ASSERT(i == 13, "st_pos = 13");

  i = st_pos(hay, ned, 14, 0);
  ASSERT(i == 18, "st_pos = 18");

  i = st_pos(hay, ned, 19, 0);
  ASSERT(i == 19, "st_pos = 19");

  i = st_pos(hay, ned, 20, 0);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&ned);
  ned = st_newc("fdusgeui", st_enc_ascii);
  i = st_pos(hay, ned, 20, 0);
  ASSERT(i == -1, "st_pos = -1");

  st_delete(&hay);
  st_delete(&ned);

  // UTF-8
  hay = st_newc(T_STR_03_REP4, st_enc_utf8);
  ned = st_newc("☃", st_enc_utf8);

  i = st_pos(hay, ned, 0, 0);
  ASSERT(i == 5, "st_pos = 5");

  i = st_pos(hay, ned, 6, 0);
  ASSERT(i == 11, "st_pos = 11");

  i = st_pos(hay, ned, 12, 0);
  ASSERT(i == 17, "st_pos = 17");

  i = st_pos(hay, ned, 18, 0);
  ASSERT(i == 23, "st_pos = 23");

  i = st_index_of(hay, ned, 1, 2);
  ASSERT(i == -1, "st_pos = -1");

  // st_start_with / st_end_with
  assert(st_start_with(hay, ned) == false);
  assert(st_end_with(hay, ned) == true);
  st_delete(&ned);

  ned = st_newc(T_STR_03, st_enc_utf8);
  assert(st_start_with(hay, ned) == true);
  assert(st_end_with(hay, ned) == true);

  st_delete(&ned);
  st_delete(&hay);

  // char_at
  string* s = st_newc(T_STR_03, st_enc_utf8);

  string* chr;
  chr = st_char_at(s, 0);
  ASSERT_STR(chr, "n", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 1);
  ASSERT_STR(chr, "i", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 2);
  ASSERT_STR(chr, "ñ", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 3);
  ASSERT_STR(chr, "o", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 4);
  ASSERT_STR(chr, " ", st_enc_utf8);
  st_delete(&chr);

  chr = st_char_at(s, 5);
  ASSERT_STR(chr, "☃", st_enc_utf8);
  st_delete(&chr);

  st_delete(&s);

  // utf32
  s = st_newc((char*)L"abc☃", st_enc_utf32be);

  chr = st_char_at(s, 3);
  ASSERT_STR(chr, L"☃", st_enc_utf32be);
  st_delete(&chr);

  chr = st_char_at(s, 1);
  ASSERT_STR(chr, L"b", st_enc_utf32be);
  st_delete(&chr);

  st_delete(&s);

  // st_rpos
  hay = st_newc(T_STR_03_REP4, st_enc_utf8);
  ned = st_newc("☃", st_enc_utf8);
  i = st_rpos(hay, ned, 0, hay->length);
  assert(i == 23);

  i = st_rpos(hay, ned, 0, -1);
  assert(i == 17);

  st_delete(&ned);
  st_delete(&hay);

  hay = st_newc("123", st_enc_utf8);
  ned = st_newc("2", st_enc_utf8);
  ASSERT(st_contains(hay, ned) == true, "123 contains 2");
  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("ABCDEFGHIJKLMNOPQRSTUVWXYZ", st_enc_ascii);
  ned = st_newc("m", st_enc_ascii);
  ASSERT(st_ipos(hay, ned, 0, 0) == 12, "m position is 12");
  ASSERT(st_irpos(hay, ned, 0, 0) == 12, "m position is 12 (rev)");

  chr = st_char_at(hay, 0);
  ASSERT_STR(chr, "A", st_enc_ascii);
  st_delete(&chr);

  chr = st_char_at(hay, 12);
  ASSERT_STR(chr, "M", st_enc_ascii);
  st_delete(&chr);

  st_delete(&hay);
  st_delete(&ned);

  //
  // delete
  //
  hay = st_newc("abcabc", st_enc_utf8);
  ned = st_newc("ab", st_enc_utf8);
  string* del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "cc", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("a", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "bcbc", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("c", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "abab", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("bc", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "aa", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("abc", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "", st_enc_utf8);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc("abca", st_enc_utf8);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, "bc", st_enc_utf8);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  hay = st_newc("1001000001", st_enc_utf8);
  ned = st_newc("1", st_enc_utf8);
  del = st_remove(hay, ned, 2, 2);
  ASSERT_STR(del, "100000001", st_enc_utf8);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  //
  // delete UTF32
  //
  hay = st_newc((const char*)L"abcabc", st_enc_utf32be);
  ned = st_newc((const char*)L"ab", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"cc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"a", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"bcbc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"c", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"abab", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"bc", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"aa", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"abc", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"", st_enc_utf32be);

  st_delete(&del);
  st_delete(&ned);
  ned = st_newc((const char*)L"abca", st_enc_utf32be);
  del = st_remove(hay, ned, 0, 0);
  ASSERT_STR(del, L"bc", st_enc_utf32be);

  st_delete(&del);
  st_delete(&hay);
  st_delete(&ned);

  //
  // delete example docs
  //
  string* haystack = st_newc("abcabcabc", st_enc_utf8);
  string* needle = st_newc("abc", st_enc_utf8);
  string* result = st_remove(haystack, needle, 2, -2);
  ASSERT_STR(result, "abcabc", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&result);

  //
  // st_replace
  //
  haystack = st_newc("test", st_enc_utf8);
  needle = st_newc("e", st_enc_utf8);
  replacement = st_newc("b", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  ASSERT_STR(result, "tbst", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  haystack = st_newc("abcabcabc", st_enc_utf8);
  needle = st_newc("abc", st_enc_utf8);
  replacement = st_newc("def", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  ASSERT_STR(result, "defdefdef", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  haystack = st_newc("abcabcabca", st_enc_utf8);
  needle = st_newc("abca", st_enc_utf8);
  replacement = st_newc("def", st_enc_utf8);
  result = st_replace(haystack, needle, replacement, 0);
  ASSERT_STR(result, "defbcdef", st_enc_utf8);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  // grow
  haystack = st_newc("aaa", st_enc_ascii);
  needle = st_newc("a", st_enc_ascii);
  replacement = st_newc("abc", st_enc_ascii);
  result = st_replace(haystack, needle, replacement, 0);
  ASSERT_STR(result, "abcabcabc", st_enc_ascii);
  st_delete(&haystack);
  st_delete(&needle);
  st_delete(&replacement);
  st_delete(&result);

  //
  // st_spn
  //
  string* subject = st_newc("22222222aaaa bbb1111 cccc", st_enc_ascii);
  string* mask = st_newc("1234", st_enc_ascii);

  ASSERT(st_spn(subject, mask, 0, 0) == 8, "st_spn 1");
  ASSERT(st_spn(subject, mask, 2, 0) == 8, "st_spn 2");
  ASSERT(st_spn(subject, mask, 2, 3) == 6, "st_spn 3");

  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("this is the test string", st_enc_ascii);
  mask = st_newc("htes ", st_enc_ascii);
  // TODO! ASSERT(st_spn(subject, mask, 8, 30) == 1, "st_spn 1");
  // printf("%ld\n", st_spn(subject, mask, 8, 0));
  // exit(1);
  ASSERT(st_spn(subject, mask, 8, 0) == 19, "st_spn 4");
  ASSERT(st_spn(subject, mask, 0, 0) == 2, "st_spn 5");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("☃龍☃龍!☃龍☃龍!", st_enc_utf8);
  mask = st_newc("☃龍", st_enc_utf8);
  ASSERT(st_spn(subject, mask, 0, 0) == 4, "st_spn 6");
  ASSERT(st_spn(subject, mask, 5, 0) == 9, "st_spn 7");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("abcd", st_enc_utf8);
  mask = st_newc("apple", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 0, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("abcd", st_enc_utf8);
  mask = st_newc("banana", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 0, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("hello", st_enc_utf8);
  mask = st_newc("l", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 2, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  subject = st_newc("hello", st_enc_utf8);
  mask = st_newc("world", st_enc_utf8);
  ASSERT(st_cspn(subject, mask, 0, 0) == 2, "st_cspn 1");
  st_delete(&subject);
  st_delete(&mask);

  //
  // st_insert
  //

  subject = st_newc("hello", st_enc_utf8);
  string* ins = st_newc("world", st_enc_utf8);
  st_insert(&subject, ins, 0);
  ASSERT_STR(subject, "worldhello", st_enc_utf8);

  st_insert(&subject, ins, 1);
  ASSERT_STR(subject, "wworldorldhello", st_enc_utf8);

  st_insert(&subject, ins, -1);
  ASSERT_STR(subject, "wworldorldhellworldo", st_enc_utf8);

  st_insert(&subject, ins, subject->length);
  ASSERT_STR(subject, "wworldorldhellworldoworld", st_enc_utf8);

  st_delete(&subject);
  st_delete(&ins);

  return 0;
}
