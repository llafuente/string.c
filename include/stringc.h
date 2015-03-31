// common headers
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef __STRINGC_TYPE__
#define __STRINGC_TYPE__

//! string length type.
typedef long int string_len_t;

// supported charsets
typedef enum {
  // ASCII
  ascii,
  // UTF-8
  utf8
} charset_t;

/// string type, use value[] at the end, so only one malloc is enough
typedef struct  {
  /// length (ASCII) atm
  string_len_t length;
  /// memory reserved
  size_t capacity;
  /// charset used
  charset_t charset;
  /// current string content, it's null-terminated
  char value[];
} string;


extern string* string_def_trim_mask;

//
// MACROS
//

#define __STRING_MEM_FREE_ADDR 0
#define __MEM_RESIZE_ADDR 1

#ifndef __STRING_ALLOCATOR
  #define __STRING_ALLOCATOR malloc
#endif

#ifndef __STRING__REALLOCATOR
  #define __STRING__REALLOCATOR realloc
#endif

#ifndef __STRING_DEALLOCATOR
  #define __STRING_DEALLOCATOR free
#endif

// add '\0' at the end of the string
void string_zeronull(string* str);

/**
 * print to stdout useful information to debug
 */
void string_debug(string* str);

/**
 * Appen src to out
 * out will be resized if needed
 *
 * @see string_concat
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
 * @return
 *   <0 a < b (length or content)
 *   =0 Both strings has same content & length.
 *   >0 a > b (length or content)
 */
int string_compare(string* a, string* b);

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

/// default charset argument trick
#define string_new(len,...) _string_new(len, (ascii, ##__VA_ARGS__) )
/// default charset argument trick
#define string_newc(chars,...) _string_newc(chars, (ascii, ##__VA_ARGS__) )
/// default charset argument trick
#define string_copyc(out, src, ...) _string_copyc(out, src, (ascii, ##__VA_ARGS__) )

/**
 * Allocate a new string
 * to edit allocator define: __STRING_ALLOCATOR
 */
extern string* _string_new(size_t len, charset_t charset);

/**
 * Allocate a new string and copy src into it.
 */
string* _string_newc(const char* src, charset_t charset);

/**
* Reallocate src with given len
* to edit allocator define: __STRING_REALLOCATOR
*/
void string_resize(string** src, size_t len);

/**
 * clone src
 * @return a new string
 */
string* string_clone(string* src);

string* string_clone_subc(char* src, size_t len, charset_t charset);

/**
 * Copy src into out
 * out can be resized
 */
void string_copy(string** out, string* src);

/**
 * Copy src into out
 * out can be resized
 */
void _string_copyc(string** out, const char* src, charset_t charset);

/**
 * delete string
 * out will be modified, pointed to __STRING_MEM_FREE_ADDR
 * to edit deallocator define: __STRING_DEALLOCATOR
 */
void string_delete(string** str);

/**
 * remove data, do no deallocate anything, just clean.
 */
void string_clear(string* str);

/**
 * Clean up operation, call this before exit.
 * Some operation require intermediate/costly objects
 * this functions take care of them
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
string* string_sub(string* str, int start, int end);

string* string_trim(string *str, string *what_str, int mode);

#define string_utf8_next_char(p) (char *)((p) + string_utf8_skip[*(unsigned char *)(p)])

#define string_utf8_jump_next(p) string_utf8_skip[*(unsigned char *)(p)]

size_t string_utf8_lenc(const char* c, size_t *capacity);

int string_is_utf8(unsigned char *str, size_t len, size_t* first_invalid_pos);

void charmask(unsigned char *input, size_t len, char *mask);

#endif
