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

//-common headers
#include <assert.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
#include "stdint-msvc2008.h"
#else
#include <stdint.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <wchar.h>

#ifndef __STRINGC_TYPE__
#define __STRINGC_TYPE__

//-TODO add static-dynamic link suppor
#define ST_EXTERN extern

/* cldoc:begin-category(stringc.h) */

// string length type.
typedef int32_t st_len_t;
// string capacity/size type.
typedef uint32_t st_size_t;
// uchar
typedef uint8_t st_uc_t;
// uchar (wide)
typedef uint32_t st_uc4_t;

// supported encodings
typedef enum st_enc_t {
  // Binary encoding for abuse in user land
  st_enc_binary,
  // ASCII
  st_enc_ascii,
  // UTF-8
  st_enc_utf8,
  // UCS-4BE, UCS-4 big endian
  st_enc_utf32be,
  // UCS-4LE, UCS-4 little endian
  st_enc_utf32le
} st_enc_t;

// string type, use value[] at the end, so only one malloc is enough
typedef struct string_s {
  // length in characters (not printable character)
  st_len_t length;
  // memory used in bytes
  st_len_t used;
  // memory reserved in bytes
  st_size_t capacity;
  // used encoding
  st_enc_t encoding;
  // current string content, it's null-terminated to be 100% compatible with
  // any c library
  char value[];
} string;

/* iterator callback type for: st_char_iterator
 *
 * [st_char_iterator](#st_char_iterator)
 */
typedef void (*st_char_itr_cb)(const string* chr, st_len_t pos,
                               const string* src);

/* iterator callback type for: st_byte_iterator
 *
 * [st_byte_iterator](#st_byte_iterator)
 */
typedef void (*st_byte_itr_cb)(st_uc_t byte, st_len_t pos, const string* src);

/* iterator callback type for: st_char_map
 * chr will be mapped in the returned string
 *
 * [st_char_map](#st_char_map)
 */
typedef void (*st_char_map_cb)(string* chr, st_len_t pos, const string* src);

//-
//- shared globals
//-
extern string* string_def_trim_mask;

//-
//- MACROS (user can override)
//-

#define __STRING_MEM_FREE_ADDR 0

#ifndef __STRING_ALLOCATOR
#define __STRING_ALLOCATOR malloc
#endif

#ifndef __STRING__REALLOCATOR
#define __STRING__REALLOCATOR realloc
#endif

#ifndef __STRING_DEALLOCATOR
#define __STRING_DEALLOCATOR free
#endif

//-
//- MACROS (good ones)
//-

#define ST_UTF8_COUNT_TRAIL(byte)                                              \
  (((byte) >= 0xc0) + ((byte) >= 0xe0) + ((byte) >= 0xf0))

#define ST_UTF8_IS_TRAIL(c) (((c)&0xc0) == 0x80)

#define ST_UTF8_IS_LEAD(c) ((st_uc_t)((c)-0xc0) < 0x3e)

#define ST_UTF8_BACK(s)                                                        \
  --s;                                                                         \
  while (ST_UTF8_IS_TRAIL(*s)) {                                               \
    --s;                                                                       \
  }

// moved to a function rather than macro
// macro *str will dereference 3 times
// this could get inlined and give better performance...
#define ST_UTF8_FOWARD(s) s += st_utf8_char_size(s)

extern const st_uc_t st_bom[];

#define ST_UTF8_HAS_BOM(s)                                                     \
  *s == st_bom[0] && *(s + 1) == st_bom[1] && *(s + 2) == st_bom[2]

// advance pointer to amount positions ASCII
#define ST_ADVANCE_ASCII(s, amount) s += amount

// advance pointer to amount positions UTF8
#define ST_ADVANCE_UTF8(s, amount)                                             \
  {                                                                            \
    st_len_t tmp = amount;                                                     \
    while (tmp--) {                                                            \
      ST_UTF8_FOWARD(s);                                                       \
    }                                                                          \
  }

// advance pointer to amount positions UCS4BE
#define ST_ADVANCE_UTF32(s, amount) s += amount * 4

// advance pointer to amount positions
#define ST_ADVANCE(s, amount, enc)                                             \
  switch (enc) {                                                               \
  case st_enc_binary:                                                          \
  case st_enc_ascii:                                                           \
    ST_ADVANCE_ASCII(s, amount);                                               \
    break;                                                                     \
  case st_enc_utf8:                                                            \
    ST_ADVANCE_UTF8(s, amount);                                                \
    break;                                                                     \
  case st_enc_utf32le:                                                         \
  case st_enc_utf32be:                                                         \
    ST_ADVANCE_UTF32(s, amount);                                               \
    break;                                                                     \
  }

#define ST_CHAR_CP_ASCII(dst, src, null_end)                                   \
  dst[0] = src[0];                                                             \
  if (null_end) {                                                              \
    dst[1] = '\0';                                                             \
  }

#define ST_CHAR_CP_UTF32(dst, src, null_end)                                   \
  dst[0] = src[0];                                                             \
  dst[1] = src[1];                                                             \
  dst[2] = src[2];                                                             \
  dst[3] = src[3];                                                             \
  if (null_end) {                                                              \
    dst[4] = '\0';                                                             \
    dst[5] = '\0';                                                             \
    dst[6] = '\0';                                                             \
    dst[7] = '\0';                                                             \
  }

#define ST_CHAR_CP_UTF8(dst, src, null_end)                                    \
  {                                                                            \
    st_uc_t c = (st_uc_t)*src;                                                 \
                                                                               \
    dst[0] = src[0];                                                           \
    if (c >= 0xc0) {                                                           \
      dst[1] = src[1];                                                         \
      if (c >= 0xe0) {                                                         \
        dst[2] = src[2];                                                       \
        if (c >= 0xf0) {                                                       \
          dst[3] = src[3];                                                     \
          if (null_end) {                                                      \
            dst[4] = '\0';                                                     \
          }                                                                    \
        } else if (null_end) {                                                 \
          dst[3] = '\0';                                                       \
        }                                                                      \
      } else if (null_end) {                                                   \
        dst[2] = '\0';                                                         \
      }                                                                        \
    } else if (null_end) {                                                     \
      dst[1] = '\0';                                                           \
    }                                                                          \
  }

#define ST_CHAR_CP(dst, src, amount, null_end)                                 \
  {                                                                            \
    st_len_t tmp = amount;                                                     \
                                                                               \
    if (null_end) {                                                            \
      dst[tmp] = '\0';                                                         \
    }                                                                          \
                                                                               \
    while (tmp--) {                                                            \
      dst[tmp] = src[tmp];                                                     \
    }                                                                          \
  }

/* cldoc:end-category() */
//-
//- utils.c
//-
/* cldoc:begin-category(utils.c) */

/* Create an ascii map
 *
 * @input Chars to map
 * @len Length of the input
 * @mask char[256] pre-allocated by user
 */
ST_EXTERN void st_charmask(const char* input, st_size_t len, char* mask);

/* Get plain string (null terminated) length in given encoding
 *
 * @return string length
 * @src Null terminated string
 * @enc Encoding
 */
ST_EXTERN st_len_t st_length(const char* src, st_enc_t enc);

/* Get plain string (null terminated) capacity in given encoding
 *
 * @return string length (do not include null terminated space)
 * @src Null terminated string
 * @enc Encoding
 */
ST_EXTERN st_size_t st_capacity(const char* src, st_enc_t enc);

/* Get capacity and length of given string in given encoding
 *
 * @src Null terminated string
 * @enc Encoding
 * @len Length
 * @capacity (do not include null terminated space)
 */
ST_EXTERN void st_get_meta(const char* src, st_enc_t enc, st_len_t* len,
                           st_size_t* capacity);
/* Print to stdout useful information to debug
 * It add zeronull at the end of the string to not overflow when printf
 *
 * @str A string
 */
ST_EXTERN void st_debug(string* str);

/* Print to stdout hex data for given plain string
 *
 * @p
 * @size
 */
ST_EXTERN void st_hexdump(const char* p, st_size_t size);

/* Validate input string.
 *
 * @return
 *    true if is valid
 *    false if not
 * @input
 * @enc
 */
ST_EXTERN bool st_validate_encoding(char* input, st_enc_t enc);

/* Returns plain string character size
 *
 * @TODO handle invalid surrogates
 *
 * @return
 *   -1, invalid lead byte
 *   1-4 character length
 * @input
 *        pointer to first char in the character (lead)
 * @enc
 */
ST_EXTERN st_len_t st_char_size_safe(const char* input, st_enc_t enc);

/* Returns plain string character size (unsafe, no error check)
 *
 * @return
 *   1-4 character length
 * @input
 *        pointer to first char in the character (lead)
 * @enc
 */
ST_EXTERN st_len_t st_char_size(const char* input, st_enc_t enc);
/* Return codepoint in given `enc`
 * @return codepoint
 * @str
 * @enc encoding
 */
ST_EXTERN st_uc4_t st_codepoint(const char* str, st_enc_t enc);

/* Return length of `codepoint` stored in `out` in given encoding.
 *
 * @return length of given encoding
 * @out buffer to store codepoint
 * @codepoint
 * @enc encoding
 */
ST_EXTERN st_len_t
    st_from_codepoint(char* out, st_uc4_t codepoint, st_enc_t enc);

/* cldoc:end-category() */
//-
//- append.c
//-
/* cldoc:begin-category(append.c) */

/* Append input to out
 *
 * [st_concat](#st_concat)
 *
 * @out
 *  out could be resized
 * @src
 *  string to append
 */
ST_EXTERN void st_append(string** out, string* src);

/* Concatenate two string and return a new one.
 *
 * [st_append](#st_append)
 *
 * @return new string
 * @first
 * @second
 */
ST_EXTERN string* st_concat(string* first, string* second);

/* cldoc:end-category() */
//-
//- bin2hex.c
//-
/* cldoc:begin-category(bin2hex.c) */

/* Returns an ASCII string containing the hexadecimal representation of str. The
 * conversion is done byte-wise with the high-nibble first.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * see [st_hex2bin](#st_hex2bin)
 *
 * @return new string with the hexadecimal representation of the given string.
 * @src A string
 */
ST_EXTERN string* st_bin2hex(const string* src);

/* cldoc:end-category() */
//-
//- compare.c
//-
/* cldoc:begin-category(compare.c) */

/* Compare two strings byte-to-byte.
 *
 * @return
 * * <0 a < b (length or content)
 * * =0 Both strings has same content & length.
 * * \>0 a > b (length or content)
 * @a
 * @b
 */
ST_EXTERN int st_compare(const string* a, const string* b);

/* @alias st_compare
 */
ST_EXTERN int st_cmp(const string* a, const string* b);

/* Compare a substring against b string
 * [st__calc_range](#st__calc_range)
 *
 * @return abc
 *   * <0 a < b (length or content)
 *   * =0 Both strings has same content & length.
 *   * >0 a > b (length or content)
 * @a
 * @b
 * @offset see st__calc_range
 * @length see st__calc_range
 */
int st_subcompare(const string* a, const string* b, st_len_t offset,
                  st_len_t length);

/* @alias st_compare
 */
ST_EXTERN int st_scmp(const string* a, const string* b, st_len_t offset,
                      st_len_t length);

/* cldoc:end-category() */
//-
//- encode.c
//-
/* cldoc:begin-category(encode.c) */

/* > TODO encode a string to given encoding
 *
 * @return new string
 * @src
 * @to_enc
 */
ST_EXTERN string* string_encode(string* src, st_enc_t to_enc);

/* cldoc:end-category() */
//-
//- to.c
//-
/* cldoc:begin-category(to.c) */

/* Convert a string replesentation of a number in a given base to number.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @return number
 * @src
 * @base
 */
ST_EXTERN double st_base2number(string* src, int base);

// binary to decimal
#define st_bin2dec(value) st_base2number(value, 2)

// octal to decimal
#define st_oct2dec(value) st_base2number(value, 8)

// hexadecimal to decimal
#define st_hex2dec(value) st_base2number(value, 16)

/* Returns a new string containing the representation of the given (unsigned)
 * number argument in given base.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @value
 * @base
 * @return new string
 */
ST_EXTERN string* st_number2base(size_t value, int base);

// decimal to binary
#define st_dec2bin(value) st_number2base(value, 2)

// decimal to octal
#define st_dec2oct(value) st_number2base(value, 8)

// decimal to hexadecimal
#define st_dec2hex(value) st_number2base(value, 16)

/* Returns a new one-character string containing the character specified
 *
 * @value
 * @enc
 * @return new string
 */
ST_EXTERN string* st_chr(st_uc4_t value, st_enc_t enc);

/* Return the code point at specified offset
 *
 * @str
 * @offset
 * @return code point
 */
st_uc4_t st_ord(const string* str, st_len_t offset);

#define st_char_code_at st_ord

/* cldoc:end-category() */
//-
//- hex2bin.c
//-
/* cldoc:begin-category(hex2bin.c) */

/* Returns a new string result of decoding a hexadecimally encoded
 * binary string
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @return new string
 * @src
 */
ST_EXTERN string* st_hex2bin(string* src);

/* cldoc:end-category() */
//-
//- iterators.c
//-
/* cldoc:begin-category(iterators.c) */

/* Passes each character in str to the given function
 *
 * @str
 * @itr_cb
 */
ST_EXTERN void st_char_iterator(const string* str, st_char_itr_cb itr_cb);
/* Create a new string with the results of calling a provided
 * function on every character in the string.
 *
 *
 * @return new string
 * @str
 * @map_cb
 *    first paramater will be used as return the modified character
 */
ST_EXTERN string* st_char_map(const string* str, st_char_map_cb map_cb);
/* Passes each bytes in str to the given function
 *
 * @str
 * @itr_cb
 */
ST_EXTERN void st_byte_iterator(const string* str, st_byte_itr_cb itr_cb);

/* Split given string by newline and passes each substring (newline won't be
 * included).
 *
 * @str
 * @itr_cb
 */
ST_EXTERN void st_line_iterator(const string* str, st_char_itr_cb itr_cb);

/* cldoc:end-category() */
//-
//- justify.c
//-
/* cldoc:begin-category(justify.c) */

/* Justify src and padded with padstr, if width is greater than src length.
 *
 * @return new string
 * @src
 * @width
 * @padstr must be only one character
 * @mode
 *   1 left align
 *   2 right align
 *   3 center align
 */
ST_EXTERN string* st_justify(const string* src, st_len_t width,
                             const string* padstr, char mode);
/* Center align src and padded with padstr, if width is greater than src length.
 *
 * @src
 * @width
 * @padstr must be only one character
 * @return new string
 */
ST_EXTERN string* st_center(const string* src, size_t width,
                            const string* padstr);
/* Right align src and padded with padstr, if width is greater than src length.
 *
 * @return new string
 * @src
 * @width
 * @padstr must be only one character
 */
ST_EXTERN string* st_right(const string* src, size_t width,
                           const string* padstr);
/* Left align src and padded with padstr, if width is greater than src length.
 *
 * @return new string
 * @src
 * @width
 * @padstr must be only one character
 */
ST_EXTERN string* st_left(const string* src, size_t width,
                          const string* padstr);

/* cldoc:end-category() */
//-
//- alloc.c
//-
/* cldoc:begin-category(alloc.c) */

/* Allocate a new empty string
 * > To edit allocator define: __STRING_ALLOCATOR
 *
 * @return new string
 * @cap
 * @enc
 */
ST_EXTERN string* st_new(st_size_t cap, st_enc_t enc);

/* Allocate a new empty with the maximum capacity needed to store given len
 * > To edit allocator define: __STRING_ALLOCATOR
 *
 * @return new string
 * @len
 * @enc
 */
ST_EXTERN string* st_new_max(st_len_t len, st_enc_t enc);

/* Allocate a new string and copy src into it.
 * The string capacity will be the minimal necessary to store src
 * > To edit allocator define: __STRING_ALLOCATOR
 *
 * @return new string
 * @src
 * @enc
 */
ST_EXTERN string* st_newc(const char* src, st_enc_t enc);

/* Reallocate src with given len
 * > to edit allocator define: __STRING_REALLOCATOR
 *
 * @return new string
 * @src source
 * @cap new capacity
 */
ST_EXTERN void st_resize(string** src, st_size_t cap);

/* Grow given string.
 * If string point to 0 -> st_new
 * if string has less capacity that needed -> st_resize
 *
 *
 * @src src
 * @cap new capacity
 * @enc
 */
ST_EXTERN void st_grow(string** out, st_size_t cap, st_enc_t enc);

/* Return a new string clone of src (same capacity)
 *
 * @return new string
 * @src source
 */
ST_EXTERN string* st_clone(const string* src);

/* Return a new string clone of src with given capacity (resize)
 *
 * @return new string
 * @src source
 */
ST_EXTERN string* st_rclone(const string* src, st_size_t cap);

/* Create a new string from given substring
 *
 * @return new string
 * @src
 * @len length in bytes
 * @enc
 */
ST_EXTERN string* st_new_subc(const char* src, st_size_t len, st_enc_t enc);

/* Copy src into out
 * > out will be resized if needed
 *
 * @out
 * @src
 */
ST_EXTERN void st_copy(string** out, const string* src);

// alias of st_copyc
#define st_cp st_copy

/* Copy src into out
 * > out will be resized if needed
 *
 * @out
 * @src
 * @enc
 */
ST_EXTERN void st_copyc(string** out, const char* src, st_enc_t enc);

// alias of st_copyc
#define st_cpc st_copyc

/* Free given string
 *
 * > `out` will be modified and pointed to 0 (no dangling)
 * >
 * > To edit deallocator define: __STRING_DEALLOCATOR
 *
 * @out
 */
ST_EXTERN void st_delete(string** out);

/* Remove data, do no deallocate anything, just clean.
 *
 * @out
 */
ST_EXTERN void st_clear(string* out);

/* Clean up operation, call this before exit.
 * Some operation require intermediate/costly objects
 * st_cleanup take care of them and free that memory
 * > can be called more than once to free memory
 */
ST_EXTERN void st_cleanup();

/* wip
 *
 */
ST_EXTERN char* st_dump(string* s);

/* cldoc:end-category() */
//-
//- repeat.c
//-
/* cldoc:begin-category(repeat.c) */

/* Returns a new string result of repeating src x times.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @src
 * @x
 * @return new string
 */
ST_EXTERN string* st_repeat(const string* src, size_t times);

// alias of st_repeat
// [st_repeat](#st_repeat)
#define st_times st_repeat

/* cldoc:end-category() */
//-
//- shuffle.c
//-
/* cldoc:begin-category(shuffle.c) */

/* Shuffle (randomize) given string
 *
 * @return new string
 * @src
 * @len How many iteration do
 */
ST_EXTERN string* st_shuffle(string* src, size_t len);

/* cldoc:end-category() */
//-
//- substring.c
//-
/* cldoc:begin-category(substring.c) */

/* Extracts the characters from a string, between two specified indices, and
 * returns the new sub string.
 * TODO review and do some comparison with PHP/Javascript usage
 *
 * @return new string
 * @str
 * @start
 *   * If non-negative, the returned string will start at the start'th position
 *in
 * string, counting from zero
 *   * If negative, the returned string will start at the start'th character
 *from
 * the end of string.
 * @end [-length, +length]
 *   * If positive, the string returned will contain at most length characters
 * beginning from start (depending on the length of string).
 *   * If is negative, then that many characters will be omitted from the end of
 * string (after the start position has been calculated when a start is
 * negative). If start denotes the position of this truncation or beyond.
 *   * If length is 0 the substring starting from start until the end of the
 * string will be returned.
 */
ST_EXTERN string* st_sub(const string* str, int start, int end);

/* cldoc:end-category() */
//-
//- trim.c
//-
/* cldoc:begin-category(trim.c) */

/* Strip whitespace (or given mask) from the beginning and/or end of a string
 *
 * @return new string
 * @str
 *   String that will be trimmed.
 * @character_mask
 *   if character_mask = 0 " \t\n\r\0\x0B" will be used
 *   String containing the list all characters that you want to be stripped.
 * @mode
 *    1 trim left
 *    2 trim right
 *    3 trim both
 */
ST_EXTERN string* st_trim(const string* str, string* character_mask, int mode);

/* shortcut to st_trim(x, x, 2)
 *
 * @return new string
 * @str
 * @character_mask
 */
ST_EXTERN string* st_chop(const string* str, string* character_mask);

/* shortcut to st_trim(x, x, 2)
 *
 * @return new string
 * @str
 * @character_mask
 */
ST_EXTERN string* st_rtrim(const string* str, string* character_mask);

/* shortcut to st_trim(x, x, 2)
 *
 * @return new string
 * @str
 * @character_mask
 */
ST_EXTERN string* st_ltrim(const string* str, string* character_mask);

/* cldoc:end-category() */
//-
//- case.c
//-
/* cldoc:begin-category(case.c) */

/* Returns a copy of `input` with the first character converted to uppercase and
 * the remainder to lowercase.
 * see [st_ucfirst](#st_ucfirst)
 * see [st_lcfirst](#st_lcfirst)
 *
 * @return new string
 * @input The input string
 */
ST_EXTERN string* st_capitalize(const string* input);

/* Returns a copy of `input` with the first character uppercased.
 * see [st_capitalize](#st_capitalize)
 * see [st_lcfirst](#st_lcfirst)
 *
 * @return new string
 * @input The input string
 */
ST_EXTERN string* st_ucfirst(const string* input);

/* Returns a copy of `input` with the first character lowercased.
 * see [st_capitalize](#st_capitalize)
 * see [st_ucfirst](#st_ucfirst)
 *
 * @return new string
 * @input The input string
 */
ST_EXTERN string* st_lcfirst(string* str);

/* Uppercase `str` and return it into `buffer` (null-terminated)
 *
 * > buffer overflow can happen, check it before
 *
 * @str
 * @buffer
 * @enc
 */
ST_EXTERN void st__char_upper(char* str, char* buffer, st_enc_t enc);

/* Make a string uppercase
 *
 * @return new string
 * @input The input string
 */
ST_EXTERN string* st_upper(const string* input);

/* Lowercase `str` and return it into `buffer` (null-terminated)
 *
 * > buffer overflow can happen, check it before
 *
 * @str
 * @buffer
 * @enc
 */
ST_EXTERN void st__char_lower(char* str, char* buffer, st_enc_t enc);

/* Make a string lowercase
 *
 * @return new string
 * @input The input string
 */
ST_EXTERN string* st_lower(const string* input);

/* cldoc:end-category() */
//-
//- search.c
//-
/* cldoc:begin-category(search.c) */

/* Find the numeric position of the first occurrence of needle in the haystack
 *string.
 * Returns -1 if the value to search never occurs.
 * [st__calc_range](#st__calc_range) to see how offset & length works
 *
 * @return
 * * \> 0 position of the first occurrence if found
 * * -1 if not found
 * @haystack
 * @needle
 * @offset
 * @length
 */
ST_EXTERN st_len_t st_pos(const string* haystack, const string* needle,
                          st_len_t offset, st_len_t length);
/* alias of [st_pos](#st_pos)
 *
 * @return
 * * \> 0 position of the first occurrence if found
 * * -1 if not found
 * @haystack
 * @needle
 * @offset
 * @length
 */
ST_EXTERN st_len_t st_index_of(const string* haystack, const string* needle,
                               st_len_t offset, st_len_t length);

/* Shortcut of [st_pos](#st_pos) that only check if it's found
 *
 * @return true if found, false otherwise
 * @haystack
 * @needle
 */
ST_EXTERN bool st_contains(const string* haystack, const string* needle);

// alias of st_contains
// [st_contains](#st_contains)
#define st_include st_contains

/* Returns if haystack starts with needle
 *
 * @return
 * true if start, false otherwise
 * @haystack
 * @needle
 */
ST_EXTERN bool st_start_with(const string* haystack, const string* needle);

/* Returns if haystack ends with needle
 *
 * @return
 * true if ends, false otherwise
 * @haystack
 * @needle
 */
ST_EXTERN bool st_end_with(const string* haystack, const string* needle);

/* returns the position of the last occurrence of a specified value in a string.
 *
 * @return
 * * -1 if not found
 * * \> -1 if found, position
 * @haystack
 * @needle
 * @offset
 * @length
 */
ST_EXTERN st_len_t st_ipos(const string* haystack, const string* needle,
                           st_len_t offset, st_len_t length);

/* Return a string with the char at given position
 *
 * @return new string
 * @src
 * @pos
 */
ST_EXTERN string* st_char_at(const string* src, st_len_t pos);

ST_EXTERN st_len_t
    st_rpos(string* haystack, string* needle, st_len_t offset, st_len_t len);

/* [st_pos](#st_pos) but case insensitive
 *
 * @return new string
 * @haystack
 * @needle
 * @offset
 * @length
 */
ST_EXTERN st_len_t st_irpos(const string* haystack, const string* needle,
                            st_len_t offset, st_len_t length);
/* Remove any occurrence of needle in haystack
 *
 * @return new string
 * @haystack
 * @needle
 * @offset
 * @length
 */
ST_EXTERN string* st_remove(const string* haystack, const string* needle,
                            st_len_t offset, st_len_t length);

/* Replace all occurrences of the search string with the replacement string
 *
 * Caveats
 * It will replace left to right always reading haystack.
 * That means replacement wont be included in needle test ever, avoid possible
 * infinite loop.
 * st_replace("abcd", "bc", "abc")
 *
 * @return new string
 * @haystack
 * @needle
 * @replacement
 * @count
 */
ST_EXTERN string* st_replace(const string* haystack, const string* needle,
                             const string* replacement, st_len_t* count);

/* Return the position of the maximum initial segment
 * of `subject` which contains only characters in `mask`.
 *
 * @return position
 * @subject
 * @mask
 */
ST_EXTERN st_len_t st_spn(const string* subject, const string* mask,
                          st_len_t offset, st_len_t length);

/* Return the position of the maximum initial segment of S
 * of `subject` which contains no characters in `mask`.
 *
 * @return position
 * @subject
 * @mask
 */
ST_EXTERN st_len_t st_cspn(const string* subject, const string* mask,
                           st_len_t offset, st_len_t length);
/* Add `ins` into `offset` position
 *
 * @return position
 * @subject
 * @mask
 */
ST_EXTERN void st_insert(string** out, const string* ins, st_len_t offset);

/* cldoc:end-category() */
//-
//- encode.c
//-
/* cldoc:begin-category(encode.c) */

/* Enconde a string into utf32
 *
 * @src
 * @return new string
 */
ST_EXTERN string* st_to_utf32(const string* src);

/* Enconde a string into utf8
 *
 * @src
 * @return new string
 */
ST_EXTERN string* st_to_utf8(const string* src);

/* cldoc:end-category() */
//-
//- picobase.c
//-
/* cldoc:begin-category(picobase.c) */

/* Uppercase unicode codepoint
 *
 * @return uppercased unicode codepoint
 * @utf32 unicode codepoint
 */
ST_EXTERN st_uc4_t st_utf32_uppercase(st_uc4_t utf32);
/* Lowercase unicode codepoint
 *
 * @return lowercased unicode codepoint
 * @utf32 unicode codepoint
 */
ST_EXTERN st_uc4_t st_utf32_lowercase(st_uc4_t utf32);

/* cldoc:end-category() */
//-
//- internal.c
//-
/* cldoc:begin-category(internal.c) */

/* Searches within the first `n` bytes of the block of memory pointed by `s` for
 *the first occurrence of `c`
 *
 * @return
 * * pointer to the first occurrent
 * * 0 if not found
 * @s
 * @c
 * @n
 */
ST_EXTERN char* st__memchr(const char* s, st_uc_t c, size_t n);
/* Returns a pointer to the first occurrence in `s1` of any of the characters
 * that are part of `s2`, or a null pointer if there are no matches.
 * The search does not include the terminating null-characters of either
 * strings, but ends there.
 * Check is done at byte level, do not use utf8 here
 *
 * @return
 * * pointer to the first occurrent
 * * 0 if not found
 * @s1 haystack
 * @s2 character list to search
 */
ST_EXTERN char* st__mempbrk(const char* s1, const char* s2);

/* Return a pointer to given string offset
 *
 * ascii example
 * | str->length | offset | return |
 * |:-----------:|:------:|:------:|
 * | 10          | 0      | str    |
 * | 10          | 5      | str + 5|
 * | 10          | -5     | str + 5|
 * | 10          | -2     | str + 8|
 * | 10          | 2      | str + 2|
 *
 * @return offset position
 * @str
 * @offset
 */
ST_EXTERN char* st__get_char_offset(const string* str, st_len_t offset);

/* Return pointer to given positions.
 *
 * Ranges (offset+length) won't be normalized, call
 *[st__calc_range](#st__calc_range) before.
 *
 * @str
 * @start_pos
 * @end_pos
 * @start
 * @end
 */
ST_EXTERN void st__get_char_range(const string* str, st_len_t start_pos,
                                  st_len_t end_pos, char** start, char** end);

/* utf32 code point to utf8 string
 *
 * @utf32
 * @utf8
 */
ST_EXTERN st_len_t st__utf32cp_to_utf8c(st_uc4_t utf32, st_uc_t* utf8);

/* Normalize offset using the same logic
 *@[st__get_char_range](#st__get_char_range)
 *
 * @return normalized offset
 * @str_length
 * @offset
 */
ST_EXTERN st_len_t st__calc_offset(st_len_t str_length, st_len_t offset);

/* Normalize given range to be useable in
 *[st__get_char_range](#st__get_char_range)
 *
 * Example:
 *
 * | str_length | offset | offset_length | out_offset | out_length |
 * |:----------:|:------:|:-------------:|:----------:|:----------:|
 * | 10         | 0      | 0             | 0          | 10         |
 * | 10         | -1     | 0             | 9          | 10         |
 * | 10         | -9     | 0             | 1          | 10         |
 * | 10         | -5     | 3             | 5          | 8          |
 * | 10         | -5     | 1             | 5          | 7          |
 * | 10         | -5     | -1            | 5          | 9          |
 * | 10         | 4      | -2            | 4          | 8          |
 *
 * @str_length
 * @offset
 *   < 0 ofsset from the end of the string
 *   >= 0 ofsset from the begining of the string
 * @length
 *   = 0 end of string
 *   < 0 from the end of the string
 *   > 0 length from offset
 */
ST_EXTERN void st__calc_range(st_len_t str_length, st_len_t* offset,
                              st_len_t* offset_length);

/* Repeat src given times in dst
 *
 * > buffer overflow can happen, check it before.
 *
 * @dst
 * @src
 * @src_len
 * @times
 */
ST_EXTERN void st__repeat(char* dst, const char* src, size_t src_len,
                          size_t times);
/* Add zero null to given position, apropiate for each enc
 *
 * @str
 * @bytepos
 * @enc
 */
ST_EXTERN void st__zeronull(char* str, size_t bytepos, st_enc_t enc);

/* Return the size of zeronull "char" in given `enc`
 *
 * @enc
 */
ST_EXTERN size_t st__zeronull_size(st_enc_t enc);

//--------------------------------//
//-        encoding group        -//
//--------------------------------//

/* cldoc:end-category() */
//-
//- utf8.c
//-
/* cldoc:begin-category(utf8.c) */

/* Return how many bytes contains given lead and -1 if it's invalid.
 *
 * @return
 * * -1 on error
 * * 1-4 if success
 * @input
 */
ST_EXTERN st_len_t st_utf8_char_size_safe(const char* input);

/* Return how many bytes contains given lead, with no error control.
 *
 * @return
 * * 1-4 if success
 * @lead_chr
 */
ST_EXTERN st_len_t st_utf8_char_size(const char* input);

/* Return how many bytes contains given lead, with no error control.
 *
 * @return
 * * 1-4 if success
 * @lead_chr
 */
ST_EXTERN st_len_t st_utf8_lead_size(st_uc_t lead);

/* Return utf8 length and capacity
 * based on glib_utf8_offset_to_pointer
 *
 * @return string length utf-8 encoded
 * @src
 * @capacity
 *   Optional, 0 means you don't want the value
 */
ST_EXTERN st_len_t st_utf8_length(const char* src, st_size_t* capacity);

/* Check if the given uchar* is a valid utf-8 sequence.
 * Return value :
 * If the string is valid utf-8, 0 is returned.
 * Else the position, starting from 1, is returned.
 *
 * Valid utf-8 sequences look like this :
 *
 * 0xxxxxxx
 *
 * 110xxxxx 10xxxxxx
 *
 * 1110xxxx 10xxxxxx 10xxxxxx
 *
 * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * Based on is_utf8 by JulienPalard, heavily modified by llafuente
 * to archieve great performance.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @return
 *   first invalid position found or 0 if success
 * @str
 * @len
 */
ST_EXTERN char* st_utf8_invalid(const st_uc_t* str, size_t len);

/* Returns if two utf8 are the same. Comparison is multibyte.
 *
 * @return true if are the same
 * @a
 * @b
 */
ST_EXTERN bool st_utf8_char_eq(char* a, char* b);

/* Returns if the given unicode code-point is valid
 *
 * @return
 * true if valid, false otherwise
 * @cp unicode codepoint
 */
ST_EXTERN bool st_utf8_valid_codepoint(st_uc4_t cp);

/* utf8 string to utf32 code point
 *
 * @return codepoint
 * @utf8
 */
ST_EXTERN st_uc4_t st_utf8_codepoint(const char* utf8);

/* parse codepoint into utf8 (NOT null terminated)
 *
 * @return bytes used by given codepoint
 * @utf8 buffer
 * @codepoint
 */
ST_EXTERN st_len_t st_utf8_from_codepoint(char* utf8, st_uc4_t codepoint);

/* cldoc:end-category() */
//-
//- ascii.c
//-
/* cldoc:begin-category(ascii.c) */

ST_EXTERN st_len_t st_ascii_length(const char* src, st_size_t* used_bytes);

ST_EXTERN st_len_t st_ascii_char_size_safe(const char* input);

ST_EXTERN st_len_t st_ascii_char_size(const char* input);

ST_EXTERN st_len_t st_ascii_lead_size(st_uc_t lead_chr);

ST_EXTERN st_uc4_t st_ascii_codepoint(const char* input);
ST_EXTERN bool st_ascii_valid_codepoint(st_uc4_t codepoint);
ST_EXTERN st_len_t st_ascii_from_codepoint(char* out, st_uc_t codepoint);

ST_EXTERN bool st_is_ascii(const char* input);

/* cldoc:end-category() */
//-
//- utf32.c
//-
/* cldoc:begin-category(utf32.c) */

ST_EXTERN st_len_t st_utf32_length(const char* src, st_size_t* used_bytes);

ST_EXTERN st_uc4_t st_utf32le_codepoint(const char* input);
ST_EXTERN st_uc4_t st_utf32be_codepoint(const char* input);
ST_EXTERN bool st_utf32_valid_codepoint(st_uc4_t codepoint);
ST_EXTERN st_len_t
    st_utf32be_from_codepoint(const char* input, st_uc4_t codepoint);
ST_EXTERN st_len_t
    st_utf32le_from_codepoint(const char* input, st_uc4_t codepoint);

ST_EXTERN st_len_t st_utf32_char_size(const char* input);
ST_EXTERN st_len_t st_utf32_lead_size(st_uc_t lead_chr);

ST_EXTERN st_len_t st_utf32le_char_size_safe(const char* input);
ST_EXTERN st_len_t st_utf32be_char_size_safe(const char* input);

ST_EXTERN st_len_t st_utf32le_char_size(const char* input);
ST_EXTERN st_len_t st_utf32be_char_size(const char* input);

/* cldoc:end-category() */

#endif
