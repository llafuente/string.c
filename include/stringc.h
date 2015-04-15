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

// common headers
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

#ifndef __STRINGC_TYPE__
#define __STRINGC_TYPE__

// TODO add static-dynamic link suppor
#define ST_EXTERN extern

/// string length type.
typedef long int st_len_t;
typedef uint8_t st_uc_t;
typedef uint32_t st_uc4_t;

/// supported encodings
typedef enum {
  st_enc_binary, ///< Binary encoding for abuse in user land
  st_enc_ascii,  ///< ASCII
  st_enc_utf8,   ///< UTF-8
  st_enc_ucs4be  ///< UCS-4BE, UCS-4 big endian
} st_enc_t;

/// string type, use value[] at the end, so only one malloc is enough
typedef struct string_s {
  /// length
  st_len_t length;
  /// used bytes
  st_len_t used;
  /// memory reserved
  size_t capacity;
  /// encoding used
  st_enc_t encoding;
  /// current string content, it's null-terminated
  char value[];
} string;

typedef void (*st_char_itr_cb)(const string* character, st_len_t pos,
                               const string* src);

typedef void (*st_byte_itr_cb)(st_uc_t byte, st_len_t pos, const string* src);

typedef void (*st_char_map_cb)(string* character, st_len_t pos,
                               const string* src);

//
// shared globals
//
extern string* string_def_trim_mask;

//
// MACROS
//

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

//
// MACROS (good ones)
//

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
#define ST_UTF8_FOWARD(s) s += st_utf8_char_size((st_uc_t)*s)

extern const st_uc_t st_bom[];

#define ST_UTF8_HAS_BOM(s)                                                     \
  *s == st_bom[0] && *(s + 1) == st_bom[1] && *(s + 2) == st_bom[2]

/// advance pointer to amount positions ASCII
#define ST_ADVANCE_ASCII(s, amount) s += amount

/// advance pointer to amount positions UTF8
#define ST_ADVANCE_UTF8(s, amount)                                             \
  {                                                                            \
    st_len_t tmp = amount;                                                     \
    while (tmp--) {                                                            \
      ST_UTF8_FOWARD(s);                                                       \
    }                                                                          \
  }

/// advance pointer to amount positions UCS4BE
#define ST_ADVANCE_UCS4BE(s, amount) s += amount * 4

/// advance pointer to amount positions
#define ST_ADVANCE(s, amount, enc)                                             \
  switch (enc) {                                                               \
  case st_enc_binary:                                                          \
  case st_enc_ascii:                                                           \
    ST_ADVANCE_ASCII(s, amount);                                               \
    break;                                                                     \
  case st_enc_utf8:                                                            \
    ST_ADVANCE_UTF8(s, amount);                                                \
    break;                                                                     \
  case st_enc_ucs4be:                                                          \
    ST_ADVANCE_UCS4BE(s, amount);                                              \
    break;                                                                     \
  }

#define ST_CHAR_CP_ASCII(dst, src, null_end)                                   \
  dst[0] = src[0];                                                             \
  if (null_end) {                                                              \
    dst[1] = '\0';                                                             \
  }

#define ST_CHAR_CP_UCS4BE(dst, src, null_end)                                  \
  dst[0] = src[0];                                                             \
  dst[1] = src[1];                                                             \
  dst[2] = src[2];                                                             \
  dst[3] = src[3];                                                             \
  if (null_end) {                                                              \
    dst[4] = '\0';                                                             \
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

//
// utils.c
//

/**
 * Create an ascii map
 *
 * @param input chars to map
 * @param len length of the input
 * @param mask char[256]
 */
ST_EXTERN void st_charmask(st_uc_t* input, size_t len, char* mask);
/**
 * get plain string (null terminated) length in given encoding
 *
 * @param src
 * @param enc
 * @return string length
 */
ST_EXTERN st_len_t st_length(const char* src, st_enc_t enc);
/**
 * Get plain string (null terminated) capacity in given encoding
 * it's just strlen alias atm.
 *
 * @param src
 * @param enc
 * @return string length
 */
ST_EXTERN size_t st_capacity(const char* src, st_enc_t enc);

/**
 * Get capacity and length of given string in given encoding
 *
 * @param src
 * @param enc
 * @param len length
 * @param bytes capacity needed to store the string (+1 if you want null
 * terminated)
 */
ST_EXTERN void st_get_meta(const char* src, st_enc_t enc, st_len_t* len,
                           st_len_t* bytes);

/**
 * add '\0' at the end of the string
 */
ST_EXTERN void st_zeronull(string* str);

/**
 * print to stdout useful information to debug
 */
ST_EXTERN void st_debug(string* str);

//
// append.c
//

/**
 * Append src to out
 *
 * @see st_concat
 * @param out
 *  out could be resized
 * @param src
 *  string to append
 */
ST_EXTERN void st_append(string** out, string* src);
/**
 * Concatenate two string and return a new one.
 *
 * @see st_append
 * @param first
 * @param second
 */
ST_EXTERN string* st_concat(string* first, string* second);

//
// bin2hex.c
//

/**
 * Returns a new string result of converting binary data
 * into hexadecimal representation
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 * @see st_hex2bin
 * @return Returns an ASCII string containing the hexadecimal representation of
 *str
 */
ST_EXTERN string* st_bin2hex(const string* src);

//
// compare.c
//

/**
 * Compare two strings byte-to-byte.
 *
 * @param a
 * @param b
 * @return
 *   <0 a < b (length or content)
 *   =0 Both strings has same content & length.
 *   >0 a > b (length or content)
 */
ST_EXTERN int st_compare(const string* a, const string* b);
/**
 * Compare a substring against a string
 *
 * @param a
 * @param b
 * @param offset
 * @param length
 * @return
 *   <0 a < b (length or content)
 *   =0 Both strings has same content & length.
 *   >0 a > b (length or content)
 */
int st_scompare(const string* a, const string* b, st_len_t offset,
                st_len_t length);

/// alias
/// @see st_compare
#define st_cmp st_compare

//
// encode.c
//

/**
 * TODO
 * encode a string to given encoding
 *
 * @param src
 * @param to_enc
 * @return new string
 */
ST_EXTERN string* string_encode(string* src, st_enc_t to_enc);

//
// to.c
//

/**
 * Convert a string replesentation of a number in a given base to number.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @param src
 * @param base
 * @return number
 */
ST_EXTERN double st_base2number(string* src, int base);

/// binary to decimal
#define st_bin2dec(value) st_number2base(value, 2)

/// octal to decimal
#define st_oct2dec(value) st_number2base(value, 8)

/// hexadecimal to decimal
#define st_hex2dec(value) st_number2base(value, 16)

/**
 * Returns a new string containing the representation of the given number
 * argument in given base.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @param value
 * @param base
 * @return new string
 */
ST_EXTERN string* st_number2base(size_t value, int base);

/// decimal to binary
#define st_dec2bin(value) st_number2base(value, 2)

/// decimal to octal
#define st_dec2oct(value) st_number2base(value, 8)

/// decimal to hexadecimal
#define st_dec2hex(value) st_number2base(value, 16)

/**
 * Returns a new one-character string containing the character specified
 *
 * @param value
 * @param enc
 * @return new string
 */
ST_EXTERN string* st_chr(st_uc4_t value, st_enc_t enc);

/**
* Return the code point at specified offset
*
* @param str
* @param offset
* @return code point
*/
size_t st_ord(const string* str, st_len_t offset);

#define st_char_code_at st_ord

//
// hex2bin.c
//

/**
 * Returns a new string result of decoding a hexadecimally encoded
 * binary string
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @param src
 * @return new string
 */
ST_EXTERN string* st_hex2bin(string* src);

//
// iterators.c
//

ST_EXTERN void st_char_iterator(const string* str, st_char_itr_cb itr_cb);

ST_EXTERN string* st_char_map(const string* str, st_char_map_cb map_cb);

ST_EXTERN void st_byte_iterator(const string* str, st_byte_itr_cb itr_cb);

/**
 * Iterate the string and call given callback with any line found
 * Line separator is '\n' and it's not include in the callback
 *
 * @param str
 * @param itr_cb
 */
ST_EXTERN void st_line_iterator(const string* str, st_char_itr_cb itr_cb);

//
// alloc.c
//

/**
 * Allocate a new empty string
 *
 * @note To edit allocator define: __STRING_ALLOCATOR
 *
 * @param cap
 * @param enc
 * @return new string
 */
ST_EXTERN string* st_new(size_t cap, st_enc_t enc);

/**
* Allocate a new empty with the maximum capacity needed to store given len
*
* @note To edit allocator define: __STRING_ALLOCATOR
*
* @param len
* @param enc
* @return new string
*/
ST_EXTERN string* st_new_max(st_len_t len, st_enc_t enc);

/**
 * Allocate a new string and copy src into it.
 * The string capacity will be the minimal necessary to store src
 *
 * @note To edit allocator define: __STRING_ALLOCATOR
 * @param src
 * @param enc
 * @return new string
 */
ST_EXTERN string* st_newc(const char* src, st_enc_t enc);

/**
 * Reallocate src with given len
 *
 * @note to edit allocator define: __STRING_REALLOCATOR
 *
 * @param src source
 * @param cap new capacity
 * @return new string
 */
ST_EXTERN void st_resize(string** src, size_t cap);

/**
 * Grow given string.
 * If string point to 0 -> st_new
 * if string has less capacity that needed -> st_resize
 *
 *
 * @param src src
 * @param cap new capacity
 * @param enc
 */
ST_EXTERN void st_grow(string** out, size_t cap, st_enc_t enc);

/**
 * Return a new string clone of src (same capacity)
 *
 * @param src source
 * @return new string
 */
ST_EXTERN string* st_clone(const string* src);

/**
 * Return a new string clone of src with given capacity
 *
 * @param src source
 * @return new string
 */
ST_EXTERN string* st_rclone(const string* src, size_t cap);

/**
 *
 */
ST_EXTERN string* st_clone_subc(const char* src, size_t len, st_enc_t enc);

/**
 * Copy src into out
 *
 * @note out will be resized if needed
 *
 * @param out
 * @param src
 */
ST_EXTERN void st_copy(string** out, string* src);

/// alias of st_copyc
#define st_cp st_copy

/**
 * Copy src into out
 *
 * @note out will be resized if needed
 * @param out
 * @param src
 * @param enc
 */
ST_EXTERN void st_copyc(string** out, const char* src, st_enc_t enc);

/// alias of st_copyc
#define st_cpc st_copyc

/**
 * Free given string
 *
 * @note pointer will be modified and pointer to 0 (no dangling)
 * @note To edit deallocator define: __STRING_DEALLOCATOR
 * @param out
 */
ST_EXTERN void st_delete(string** out);

/**
 * Remove data, do no deallocate anything, just clean.
 * @param out
 */
ST_EXTERN void st_clear(string* out);

/**
 * Clean up operation, call this before exit.
 * Some operation require intermediate/costly objects
 * st_cleanup take care of them and free that memory
 *
 * @note can be called more than once to free memory
 */
ST_EXTERN void st_cleanup();

//
// repeat.c
//

/**
 * Returns a new string result of repeating src x times.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @param src
 * @param x
 * @return new string
 */
ST_EXTERN string* st_repeat(const string* src, size_t x);

//
// shuffle.c
//

/**
 * Shuffle (randomize) given string
 *
 * @param src
 * @param len How many iteration do
 * @return new string
 */
ST_EXTERN string* st_shuffle(string* src, size_t len);

//
// substring.c
//

/**
 * Extracts the characters from a string, between two specified indices, and
 *returns the new sub string.
 *
 * @param str
 * @param start
 *   If non-negative, the returned string will start at the start'th position in
 *string, counting from zero
 *   If negative, the returned string will start at the start'th character from
 *the end of string.
 * @param end [-length, +length]
 *   If positive, the string returned will contain at most length characters
 *beginning from start (depending on the length of string).
 *   If is negative, then that many characters will be omitted from the end of
 *string (after the start position has been calculated when a start is
 *negative). If start denotes the position of this truncation or beyond.
 *   If length is 0 the substring starting from start until the end of the
 *string will be returned.
 * @return new string
 */
ST_EXTERN string* st_sub(const string* str, int start, int end);

//
// trim.c
//

/**
 * Strip whitespace (or given mask) from the beginning and/or end of a string
 *
 * @param str
 *   String that will be trimmed.
 * @param character_mask
 *   if character_mask = 0 " \t\n\r\0\x0B" will be used
 *   String containing the list all characters that you want to be stripped.
 * @param mode
 *    1 trim left
 *    2 trim right
 *    3 trim both
 * @return new string
 */
ST_EXTERN string* st_trim(const string* str, string* character_mask, int mode);

/// alias st_trim (right only)
#define st_chop(str, character_mask) st_trim(str, character_mask, 2)

/// alias st_trim (right only)
#define st_rtrim(str, character_mask) st_trim(str, character_mask, 2)

/// alias st_trim (left only)
#define st_ltrim(str, character_mask) st_trim(str, character_mask, 1)

//
// utf8.c
//

/**
 * Return how many bytes contains given lead and -1 if it's invalid.
 *
 * @param lead_chr
 * @return
 *  -1 on error
 *  1-4 if success
 */
ST_EXTERN int st_utf8_char_size_safe(unsigned char lead_chr);

/**
* Return how many bytes contains given lead, with no error control.
*
* @param lead_chr
* @return
*  1-4 if success
*/
ST_EXTERN st_len_t st_utf8_char_size(st_uc_t byte);

/**
 * Return utf8 length and capacity
 * based on glib_utf8_offset_to_pointer
 *
 * @param src
 * @param out_capacity
 *   Optional, 0 means you don't want the value
 * @return string length utf-8 encoded
 */
ST_EXTERN size_t st_utf8_length(const char* src, size_t* out_capacity);

/**
 * Check if the given unsigned char * is a valid utf-8 sequence.
 * Return value :
 * If the string is valid utf-8, 0 is returned.
 * Else the position, starting from 1, is returned.
 *
 * Valid utf-8 sequences look like this :
 * 0xxxxxxx
 * 110xxxxx 10xxxxxx
 * 1110xxxx 10xxxxxx 10xxxxxx
 * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * Based on is_utf8 by JulienPalard, heavily modified by llafuente
 * to archieve great performance.
 *
 * based on PHP (https://github.com/php/php-src/blob/master/LICENSE)
 *
 * @param str
 * @param len
 * @return
 *   first invalid position found or 0 if success
 */
ST_EXTERN char* st_utf8_invalid(const unsigned char* str, size_t len);

/**
 * Returns if two utf8 are the same. Comparison is multibyte.
 *
 * @param a
 * @param b
 * @return true if are the same
 */
ST_EXTERN bool st_utf8_char_eq(char* a, char* b);

//
// case.c
//

ST_EXTERN void string_capitalize(string* str);

ST_EXTERN string* st_upper(string* src);
ST_EXTERN string* st_lower(string* src);

//
// search.c
//

/**
 * Returns the position of the first occurrence of needle
 * in haystack.
 * Returns -1 if the value to search never occurs.
 *
 * @param haystack
 * @param needle
 * @param offset
 * @param len
 * @return
 *  > 0 position of the first occurrence if found
 *  -1 if not found
 */
ST_EXTERN st_len_t
    st_pos(string* haystack, string* needle, st_len_t offset, st_len_t len);

/// alias of st_pos
/// @see st_pos
#define st_index_of st_pos

ST_EXTERN bool st_contains(string* haystack, string* needle);

/// alias of st_contains
/// @see st_contains
#define st_include st_contains

/**
 * Returns if haystack starts with needle
 *
 * @param haystack
 * @param needle
 * @return
 */
ST_EXTERN bool st_start_with(string* haystack, string* needle);

/**
 * Returns if haystack ends with needle
 *
 * @param haystack
 * @param needle
 * @return
 */
ST_EXTERN bool st_end_with(string* haystack, string* needle);

ST_EXTERN st_len_t st_ipos(string* haystack, string* needle, st_len_t offset);

/**
 * Return a string with the char at given position
 *
 * @param src
 * @param pos
 * @return new string
 */
ST_EXTERN string* st_char_at(const string* src, st_len_t pos);

ST_EXTERN st_len_t
    st_rpos(string* haystack, string* needle, st_len_t offset, st_len_t len);

//
// encode.c
//

/**
 * enconde a string into utf32
 *
 * @param src
 * @return new string
 */
ST_EXTERN string* st_to_utf32(const string* src);

/**
 * enconde a string into utf8
 *
 * @param src
 * @return new string
 */
ST_EXTERN string* st_to_utf8(const string* src);

//
// picobase.c
//

ST_EXTERN st_uc4_t st_utf32_uppercase(st_uc4_t utf32);
ST_EXTERN st_uc4_t st_utf32_lowercase(st_uc4_t utf32);

//
// internal.c
//

/**
 * binary search to find the start position of needle inside haystack.
 *
 * @param s
 * @param c
 * @param n
 */
ST_EXTERN char* st__memchr(const char* s, st_uc_t c, size_t n);

ST_EXTERN char* st__mempbrk(const char* s1, const char* s2);

/**
 * Return a pointer to given string offset
 *
 * @example
 * string[10] - offset[0] --> 0
 * string[10] - offset[5] --> 5
 * string[10] - offset[-5] --> 5
 * string[10] - offset[-2] --> 8
 * string[10] - offset[2] --> 2
 *
 * @param str
 * @param offset
 * @return offset position
 */
ST_EXTERN char* st__get_char_offset(string* str, st_len_t offset);

/**
 * Find given range in the string.
 * Ranges (offset+length) won't be normalized, call st__calc_range before.
 *
 * @param str
 * @param offset
 * @param len
 */
ST_EXTERN void st__get_char_range(string* str, st_len_t offset, st_len_t length,
                                  char** start, char** end);

/**
 * utf8 string to utf32 code point
 *
 * @param utf8
 */
ST_EXTERN st_uc4_t st__utf8c_to_utf32cp(st_uc_t* utf8);

/**
 * utf32 code point to utf8 string
 *
 * @param utf32
 * @param utf8
 */
ST_EXTERN st_len_t st__utf32cp_to_utf8c(st_uc4_t utf32, st_uc_t* utf8);

/*
 * Normalize given range to be useable in st__get_char_range
 *
 * @param str_length
 * @param offset
 *   < 0 ofsset from the end of the string
 *   >= 0 ofsset from the begining of the string
 * @param length
 *   = 0 end of string
 *   < 0 from the end of the string
 *   > 0 length from offset
 */
ST_EXTERN void st__calc_range(st_len_t str_length, st_len_t* offset,
                              st_len_t* offset_length);

#endif
