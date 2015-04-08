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
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#ifndef __STRINGC_TYPE__
#define __STRINGC_TYPE__

/// string length type.
typedef long int string_len_t;

/// supported charsets
typedef enum {
  string_enc_ascii, ///< ASCII
  string_enc_utf8, ///< UTF-8
  string_enc_ucs4be ///< UCS-4BE, UCS-4 big endian
} charset_t;

/// string type, use value[] at the end, so only one malloc is enough
typedef struct  {
  /// length
  string_len_t length;
  /// used bytes
  string_len_t used;
  /// memory reserved
  size_t capacity;
  /// charset used
  charset_t charset;
  /// current string content, it's null-terminated
  char value[];
} string;

typedef void (*string_citr)(string* character, string_len_t pos, string* src);

//
// shared globals
//
extern string* string_def_trim_mask;
extern const char string_utf8_skip_data[256];
extern const char * const string_utf8_skip;

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

#define STRING_COPY_CHARS(src, dst, i) \
{ \
  const char* __end = src + i; \
  while (src < __end) { \
    *dst = *src; \
    ++src; \
    ++dst; \
  } \
}

#define STRING_LOOP(string_val, itr) \
const char* itr = string_val->value ; \
const char* __end = itr + string_val->used; \
while (itr < __end)

#define STRING_LOOP_START(string_val, itr, start) \
const char* itr = string_val->value + start; \
const char* __end = itr + string_val->used - start; \
while (itr < __end)

#define STRING_GET_CHAR_DATA(src, enc, len, used) \
string_len_t len; \
size_t used; \
if (enc == string_enc_ascii) { \
  len = used = strlen(src); \
} else if (enc == string_enc_utf8) { \
  len = string_utf8_lenc((const char*) src, &used); \
} else { \
  assert(false); \
} \

string_len_t string_length(char* src, charset_t enc);
size_t string_capacity(char* src, charset_t enc);

// add '\0' at the end of the string
void string_zeronull(string* str);

/**
 * print to stdout useful information to debug
 */
void string_debug(string* str);

/**
 * Append src to out
 *
 * @see string_concat
 * @param out
 *  out could be resized
 * @param src
 *  string to append
 */
void string_append(string** out, string* src);

/**
 * Treat the string as binary data and convert it into hexadecimal representation
 * The conversion is done byte-wise with the high-nibble first.
 *
 * @license https://github.com/php/php-src/blob/master/LICENSE
 * @see string_hex2bin
 * @return Returns an ASCII string containing the hexadecimal representation of str
 */
string* string_bin2hex(const string* src);

/**
 * Compare two strings byte-to-byte.
 *
 * @return
 *   <0 a < b (length or content)
 *   =0 Both strings has same content & length.
 *   >0 a > b (length or content)
 */
int string_compare(string* a, string* b);

/**
 * encode a string to given charset
 * @return a new string
 */
string* string_encode(string* src, charset_t to_charset);

/**
 * Convert a string replesentation of a number in a given base to number.
 *
 * @license https://github.com/php/php-src/blob/master/LICENSE
 */
double string_from_base(string *src, int base);

/**
 * Convert a number to the string representation in given base.
 * @return a new string (remember to string_delete!)
 *
 * @license https://github.com/php/php-src/blob/master/LICENSE
 */
string* string_from_number(size_t value, int base);

/**
 * treat the string as an hexadeciaml data [0-9a-f] and
 * conver it to binary data.
 *
 * @license https://github.com/php/php-src/blob/master/LICENSE
 */
string *string_hex2bin(string *src);

void string_itr_chars(const string* str, string_citr itr_cb);

/**
 * Allocate a new string
 * to edit allocator define: __STRING_ALLOCATOR
 *
 */
extern string* string_new(size_t len, charset_t charset);

/**
 * Allocate a new string and copy src into it.
 *
 */
string* string_newc(const char* src, charset_t charset);

/**
* Reallocate src with given len
* to edit allocator define: __STRING_REALLOCATOR
*
*/
void string_resize(string** src, size_t len);

/**
 * clone src
 * @return a new string
 *
 */
string* string_clone(string* src);
/**
 *
 */
string* string_clone_subc(char* src, size_t len, charset_t charset);

/**
 * Copy src into out
 * out can be resized
 *
 */
void string_copy(string** out, string* src);

/**
 * Copy src into out
 * out can be resized
 *
 */
void string_copyc(string** out, const char* src, charset_t charset);

/**
 * delete string
 * out will be modified, pointed to 0
 * to edit deallocator define: __STRING_DEALLOCATOR
 *
 */
void string_delete(string** str);
/**
 *
 */
bool string_char(string** out, const string* str, string_len_t pos);

/**
 * remove data, do no deallocate anything, just clean.
 *
 */
void string_clear(string* str);

/**
 * Clean up operation, call this before exit.
 * Some operation require intermediate/costly objects
 * this functions take care of them
 *
 */
void string_cleanup();

/**
 * Repeat src x times
 *
 * @license https://github.com/php/php-src/blob/master/LICENSE
 */
string* string_repeat(string* src, size_t x);

/**
 * Shuffle (randomize) given string
 *
 * @param len How many iteration do
 */
string* string_shuffle(string* src, size_t len);

/**
 * Create a substring
 *
 * @param start
 *   If non-negative, the returned string will start at the start'th position in string, counting from zero
 *   If negative, the returned string will start at the start'th character from the end of string.
 * @param end [-length, +length]
 *   If positive, the string returned will contain at most length characters beginning from start (depending on the length of string).
 *   If is negative, then that many characters will be omitted from the end of string (after the start position has been calculated when a start is negative). If start denotes the position of this truncation or beyond.
 *   If length is 0 the substring starting from start until the end of the string will be returned.
 */
string* string_sub(const string* str, int start, int end);

/**
 * Strip whitespace (or other characters) from the beginning and/or end of a string
 *
 * @param str
 *   String that will be trimmed.
 * @param what_str
 *   if 0 what_str = " \t\n\r\0\x0B"
 *   String containing the list all characters that you want to be stripped.
 * @param mode
 *    1 trim left
 *    2 trim right
 *    3 trim both
 * @return a new string
 */
string* string_trim(string *str, string *character_mask, int mode);

#define string_utf8_next_char(p) (char *)((p) + string_utf8_skip[*(unsigned char *)(p)])

#define string_utf8_jump_next(p) string_utf8_skip[*(unsigned char *)(p)]

/**
 * Return utf8 length and capacity
 * based on glib_utf8_offset_to_pointer
 *
 * @param src
 * @param out_capacity
 *   Optional, 0 means you don't want the value
 * @return string length utf-8 encoded
 */
size_t string_utf8_lenc(const char* src, size_t *out_capacity);

/**
 * Check if the given unsigned char * is a valid utf-8 sequence.
 * Return value :
 * If the string is valid utf-8, 0 is returned.
 * Else the position, starting from 1, is returned.
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
 * @license https://github.com/JulienPalard/is_utf8/blob/master/COPYRIGHT
 * @param str
 * @param len
 * @return
 *   first invalid position found or 0 if success
 */
char* string_utf8_invalid(const unsigned char *str, size_t len);

/**
 * Create a mask[256] for given ASCII input
 */
void string_charmask(unsigned char *input, size_t len, char *mask);

void string_capitalize(string* str);

#endif
