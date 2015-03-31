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

/// string length type.
typedef long int string_len_t;

/// charsets supportted
typedef enum {
  ascii,
  utf8
} charset_t;

/// string type
typedef struct  {
  string_len_t length;
  size_t capacity;
  charset_t charset;
  char value[];
} string;


extern string* string_def_trim_mask;

//
// MACROS
//

#define __MEM_FREE_ADDR 0
#define __MEM_RESIZE_ADDR 1

#define __string_allocator malloc
#define __string_reallocator realloc
#define __string_deallocator free


void string_zeronull(string* str);

/**
 * print to stdout useful information to debug
 */
void string_debug(string* str);

void string_append(string** out, string* src);

string* string_bin2hex(const string* old);

int string_compare(string* a, string* b);

double string_from_base(string *str, int base);

string* string_from_number(size_t value, int base);

string *string_hex2bin(string *src);

/// default charset argument trick
#define string_new(len,...) _string_new(len, (ascii, ##__VA_ARGS__) )
/// default charset argument trick
#define string_newc(chars,...) _string_newc(chars, (ascii, ##__VA_ARGS__) )
/// default charset argument trick
#define string_copyc(out, src, ...) _string_copyc(out, src, (ascii, ##__VA_ARGS__) )

string* _string_new(size_t len, charset_t charset);

string* _string_newc(const char* chars, charset_t charset);

void string_resize(string** src, size_t len);

string* string_clone(string* src);

string* string_clonec(const char* src, size_t len);

void string_copy(string** out, string* src);

void _string_copyc(string** out, const char* src, charset_t charset);

void string_delete(string** str);

void string_clear(string* str);

void string_cleanup();

string* string_repeat(string* input, size_t mult);

string* string_shuffle(string* src, size_t len);

string* string_sub(string* str, int start, int end);

string* string_trim(string *str, string *what_str, int mode);

#define string_utf8_next_char(p) (char *)((p) + string_utf8_skip[*(unsigned char *)(p)])

#define string_utf8_jump_next(p) string_utf8_skip[*(unsigned char *)(p)]

size_t string_utf8_lenc(const char* c);

int string_is_utf8(unsigned char *str, size_t len, size_t* first_invalid_pos);

void charmask(unsigned char *input, size_t len, char *mask);

#endif
