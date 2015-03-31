#include "string-type.h"
#include "stringc.h"
#include <stdlib.h>
#include <string.h>

#define __MEM_FREE_ADDR 0
#define __MEM_RESIZE_ADDR 1

#define __string_allocator malloc
#define __string_reallocator realloc
#define __string_deallocator free


#define string_new(len,...) _string_new(len, (ascii, ##__VA_ARGS__) )

string* _string_new(size_t len, charset_t charset);

#define string_newc(chars,...) _string_newc(chars, (ascii, ##__VA_ARGS__) )

string* _string_newc(const char* chars, charset_t charset);

void string_resize(string** src, size_t len);

string* string_clone(string* src);

string* string_clonec(const char* src, size_t len);

void string_copy(string** out, string* src);

#define string_copyc(out, src, ...) _string_copyc(out, src, (ascii, ##__VA_ARGS__) )

void _string_copyc(string** out, const char* src, charset_t charset);

void string_delete(string** str);

void string_clear(string* str);

void string_cleanup();
