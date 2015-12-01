# API
## TOC
* [st\_byte\_itr\_cb](#st_byte_itr_cb)
* [st\_char\_itr\_cb](#st_char_itr_cb)
* [st\_char\_map\_cb](#st_char_map_cb)
* [st\_enc\_t](#st_enc_t)
* [st\_free\_func](#st_free_func)
* [st\_len\_t](#st_len_t)
* [st\_malloc\_func](#st_malloc_func)
* [st\_realloc\_func](#st_realloc_func)
* [st\_size\_t](#st_size_t)
* [st\_uc4\_t](#st_uc4_t)
* [st\_uc\_t](#st_uc_t)
* [st\_enc\_t](#st_enc_t)
* [string](#string)
* [st\_\_calc\_offset](#st__calc_offset)
* [st\_\_calc\_range](#st__calc_range)
* [st\_\_char\_lower](#st__char_lower)
* [st\_\_char\_upper](#st__char_upper)
* [st\_\_free](#st__free)
* [st\_\_get\_char\_offset](#st__get_char_offset)
* [st\_\_get\_char\_range](#st__get_char_range)
* [st\_\_malloc](#st__malloc)
* [st\_\_memchr](#st__memchr)
* [st\_\_mempbrk](#st__mempbrk)
* [st\_\_realloc](#st__realloc)
* [st\_\_repeat](#st__repeat)
* [st\_\_utf32cp\_to\_utf8c](#st__utf32cp_to_utf8c)
* [st\_\_zeronull](#st__zeronull)
* [st\_\_zeronull\_size](#st__zeronull_size)
* [st\_append](#st_append)
* [st\_append\_c](#st_append_c)
* [st\_append\_char](#st_append_char)
* [st\_ascii\_char\_size](#st_ascii_char_size)
* [st\_ascii\_char\_size\_safe](#st_ascii_char_size_safe)
* [st\_ascii\_codepoint](#st_ascii_codepoint)
* [st\_ascii\_from\_codepoint](#st_ascii_from_codepoint)
* [st\_ascii\_lead\_size](#st_ascii_lead_size)
* [st\_ascii\_length](#st_ascii_length)
* [st\_ascii\_valid\_codepoint](#st_ascii_valid_codepoint)
* [st\_base2number](#st_base2number)
* [st\_bin2hex](#st_bin2hex)
* [st\_byte\_iterator](#st_byte_iterator)
* [st\_capacity](#st_capacity)
* [st\_capitalize](#st_capitalize)
* [st\_center](#st_center)
* [st\_char\_at](#st_char_at)
* [st\_char\_iterator](#st_char_iterator)
* [st\_char\_map](#st_char_map)
* [st\_char\_size](#st_char_size)
* [st\_char\_size\_safe](#st_char_size_safe)
* [st\_charmask](#st_charmask)
* [st\_chop](#st_chop)
* [st\_chr](#st_chr)
* [st\_clear](#st_clear)
* [st\_clone](#st_clone)
* [st\_cmp](#st_cmp)
* [st\_codepoint](#st_codepoint)
* [st\_compare](#st_compare)
* [st\_concat](#st_concat)
* [st\_concat\_random](#st_concat_random)
* [st\_contains](#st_contains)
* [st\_copy](#st_copy)
* [st\_copyc](#st_copyc)
* [st\_cspn](#st_cspn)
* [st\_debug](#st_debug)
* [st\_delete](#st_delete)
* [st\_dump](#st_dump)
* [st\_end\_with](#st_end_with)
* [st\_escape](#st_escape)
* [st\_from\_codepoint](#st_from_codepoint)
* [st\_get\_meta](#st_get_meta)
* [st\_grow](#st_grow)
* [st\_hex2bin](#st_hex2bin)
* [st\_hexdump](#st_hexdump)
* [st\_index\_of](#st_index_of)
* [st\_insert](#st_insert)
* [st\_ipos](#st_ipos)
* [st\_irpos](#st_irpos)
* [st\_is\_ascii](#st_is_ascii)
* [st\_justify](#st_justify)
* [st\_lcfirst](#st_lcfirst)
* [st\_left](#st_left)
* [st\_length](#st_length)
* [st\_line\_iterator](#st_line_iterator)
* [st\_lower](#st_lower)
* [st\_ltrim](#st_ltrim)
* [st\_memfree](#st_memfree)
* [st\_new](#st_new)
* [st\_new\_max](#st_new_max)
* [st\_new\_subc](#st_new_subc)
* [st\_newc](#st_newc)
* [st\_number2base](#st_number2base)
* [st\_ord](#st_ord)
* [st\_pos](#st_pos)
* [st\_rclone](#st_rclone)
* [st\_remove](#st_remove)
* [st\_repeat](#st_repeat)
* [st\_replace](#st_replace)
* [st\_replace\_allocators](#st_replace_allocators)
* [st\_resize](#st_resize)
* [st\_right](#st_right)
* [st\_rpos](#st_rpos)
* [st\_rtrim](#st_rtrim)
* [st\_scmp](#st_scmp)
* [st\_shuffle](#st_shuffle)
* [st\_spn](#st_spn)
* [st\_start\_with](#st_start_with)
* [st\_sub](#st_sub)
* [st\_subcompare](#st_subcompare)
* [st\_to\_utf32](#st_to_utf32)
* [st\_to\_utf8](#st_to_utf8)
* [st\_trim](#st_trim)
* [st\_ucfirst](#st_ucfirst)
* [st\_unescape](#st_unescape)
* [st\_upper](#st_upper)
* [st\_utf32\_char\_size](#st_utf32_char_size)
* [st\_utf32\_lead\_size](#st_utf32_lead_size)
* [st\_utf32\_length](#st_utf32_length)
* [st\_utf32\_lowercase](#st_utf32_lowercase)
* [st\_utf32\_uppercase](#st_utf32_uppercase)
* [st\_utf32\_valid\_codepoint](#st_utf32_valid_codepoint)
* [st\_utf32be\_char\_size](#st_utf32be_char_size)
* [st\_utf32be\_char\_size\_safe](#st_utf32be_char_size_safe)
* [st\_utf32be\_codepoint](#st_utf32be_codepoint)
* [st\_utf32be\_from\_codepoint](#st_utf32be_from_codepoint)
* [st\_utf32le\_char\_size](#st_utf32le_char_size)
* [st\_utf32le\_char\_size\_safe](#st_utf32le_char_size_safe)
* [st\_utf32le\_codepoint](#st_utf32le_codepoint)
* [st\_utf32le\_from\_codepoint](#st_utf32le_from_codepoint)
* [st\_utf8\_char\_eq](#st_utf8_char_eq)
* [st\_utf8\_char\_size](#st_utf8_char_size)
* [st\_utf8\_char\_size\_safe](#st_utf8_char_size_safe)
* [st\_utf8\_codepoint](#st_utf8_codepoint)
* [st\_utf8\_from\_codepoint](#st_utf8_from_codepoint)
* [st\_utf8\_invalid](#st_utf8_invalid)
* [st\_utf8\_lead\_size](#st_utf8_lead_size)
* [st\_utf8\_length](#st_utf8_length)
* [st\_utf8\_valid\_codepoint](#st_utf8_valid_codepoint)
* [st\_validate\_encoding](#st_validate_encoding)
* [string\_encode](#string_encode)

## Types

<a name="st_byte_itr_cb"></a>
### typedef st\_byte\_itr\_cb void (unsigned char, int, const string_s *) *

iterator callback type for: st_byte_iterator
[st_byte_iterator](#st_byte_iterator)


---

<a name="st_char_itr_cb"></a>
### typedef st\_char\_itr\_cb void (const string_s *, int, const string_s *) *

iterator callback type for: st_char_iterator
[st_char_iterator](#st_char_iterator)


---

<a name="st_char_map_cb"></a>
### typedef st\_char\_map\_cb void (string_s *, int, const string_s *) *

iterator callback type for: st_char_map
chr will be mapped in the returned string

[st_char_map](#st_char_map)


---

<a name="st_enc_t"></a>
### typedef st\_enc\_t st_enc_t

supported encodings


---

<a name="st_free_func"></a>
### typedef st\_free\_func void (void *) *



---

<a name="st_len_t"></a>
### typedef st\_len\_t int32_t

string length type.


---

<a name="st_malloc_func"></a>
### typedef st\_malloc\_func void *(unsigned long) *



---

<a name="st_realloc_func"></a>
### typedef st\_realloc\_func void *(void *, unsigned long) *



---

<a name="st_size_t"></a>
### typedef st\_size\_t uint32_t

string capacity/size type.


---

<a name="st_uc4_t"></a>
### typedef st\_uc4\_t uint32_t

uchar (wide)


---

<a name="st_uc_t"></a>
### typedef st\_uc\_t uint8_t

uchar


---


## Enums

<a name="st_enc_t"></a>
### enum st\_enc\_t



##### Values

* *st\_enc\_binary*

  Binary encoding for abuse in user land
  
* *st\_enc\_ascii*

  ASCII
  
* *st\_enc\_utf8*

  UTF-8
  
* *st\_enc\_utf32be*

  UCS-4BE, UCS-4 big endian
  
* *st\_enc\_utf32le*

  UCS-4LE, UCS-4 little endian
  

---


## Structs

<a name="string"></a>
### struct string

string type, use value[] at the end, so only one malloc is enough


##### Fields

* `st_len_t` *length*

  length in characters (not printable character)
  
* `st_len_t` *used*

  memory used in bytes
  
* `st_size_t` *capacity*

  memory reserved in bytes
  
* `st_enc_t` *encoding*

  used encoding
  
* `char []` *value*

  current string content, it's null-terminated to be 100% compatible with
  any c library

---


## Functions

<a name="st__calc_offset"></a>
### st\_len\_t st\_\_calc\_offset(st\_len\_t str_length, st\_len\_t offset)

Normalize offset using the same logic


##### Arguments (2)

* `st_len_t` *str\_length*

* `st_len_t` *offset*


---

<a name="st__calc_range"></a>
### void st\_\_calc\_range(st\_len\_t str_length, st\_len\_t \* offset, st\_len\_t \* offset_length)

Normalize given range to be useable in
[st__get_char_range](#st__get_char_range)

Example:

| str_length | offset | offset_length | out_offset | out_length |
|:----------:|:------:|:-------------:|:----------:|:----------:|
| 10         | 0      | 0             | 0          | 10         |
| 10         | -1     | 0             | 9          | 10         |
| 10         | -9     | 0             | 1          | 10         |
| 10         | -5     | 3             | 5          | 8          |
| 10         | -5     | 1             | 5          | 7          |
| 10         | -5     | -1            | 5          | 9          |
| 10         | 4      | -2            | 4          | 8          |



##### Arguments (3)

* `st_len_t` *str\_length*

* `st_len_t *` *offset*

* `st_len_t *` *offset\_length*


---

<a name="st__char_lower"></a>
### void st\_\_char\_lower(char \* str, char \* buffer, st\_enc\_t enc)

Lowercase `str` and return it into `buffer` (null-terminated)
> buffer overflow can happen, check it before



##### Arguments (3)

* `char *` *str*

* `char *` *buffer*

* `st_enc_t` *enc*


---

<a name="st__char_upper"></a>
### void st\_\_char\_upper(char \* str, char \* buffer, st\_enc\_t enc)

Uppercase `str` and return it into `buffer` (null-terminated)
> buffer overflow can happen, check it before



##### Arguments (3)

* `char *` *str*

* `char *` *buffer*

* `st_enc_t` *enc*


---

<a name="st__free"></a>
### void st\_\_free(void \* ptr)

Internal to free call that check for overriden allocators


##### Arguments (1)

* `void *` *ptr*


---

<a name="st__get_char_offset"></a>
### char \* st\_\_get\_char\_offset(const string\* str, st\_len\_t offset)

Return a pointer to given string offset
ascii example
| str->length | offset | return |
|:-----------:|:------:|:------:|
| 10          | 0      | str    |
| 10          | 5      | str + 5|
| 10          | -5     | str + 5|
| 10          | -2     | str + 8|
| 10          | 2      | str + 2|



##### Return: char \*

offset position

##### Arguments (2)

* `const string*` *str*

* `st_len_t` *offset*


---

<a name="st__get_char_range"></a>
### void st\_\_get\_char\_range(const string\* str, st\_len\_t start_pos, st\_len\_t end_pos, char \*\* start, char \*\* end)

Return pointer to given positions.
Ranges (offset+length) won't be normalized, call
[st__calc_range](#st__calc_range) before.



##### Arguments (5)

* `const string*` *str*

* `st_len_t` *start\_pos*

* `st_len_t` *end\_pos*

* `char **` *start*

* `char **` *end*


---

<a name="st__malloc"></a>
### void \* st\_\_malloc(size\_t size)

Internal to malloc call that check for overriden allocators


##### Arguments (1)

* `size_t` *size*


---

<a name="st__memchr"></a>
### char \* st\_\_memchr(const char\* s, st\_uc\_t c, size\_t n)

Searches within the first `n` bytes of the block of memory pointed by `s` for
the first occurrence of `c`



##### Return: char \*

* pointer to the first occurrent
* 0 if not found

##### Arguments (3)

* `const char*` *s*

* `st_uc_t` *c*

* `size_t` *n*


---

<a name="st__mempbrk"></a>
### char \* st\_\_mempbrk(const char\* s1, const char\* s2)

Returns a pointer to the first occurrence in `s1` of any of the characters
that are part of `s2`, or a null pointer if there are no matches.
The search does not include the terminating null-characters of either
strings, but ends there.
Check is done at byte level, do not use utf8 here



##### Return: char \*

* pointer to the first occurrent
* 0 if not found

##### Arguments (2)

* `const char*` *s1*

* `const char*` *s2*


---

<a name="st__realloc"></a>
### void \* st\_\_realloc(void \* ptr, size\_t size)

Internal to realloc call that check for overriden allocators


##### Arguments (2)

* `void *` *ptr*

* `size_t` *size*


---

<a name="st__repeat"></a>
### void st\_\_repeat(char \* dst, const char\* src, size\_t src_len, size\_t times)

Repeat src given times in dst
> buffer overflow can happen, check it before.



##### Arguments (4)

* `char *` *dst*

* `const char*` *src*

* `size_t` *src\_len*

* `size_t` *times*


---

<a name="st__utf32cp_to_utf8c"></a>
### st\_len\_t st\_\_utf32cp\_to\_utf8c(st\_uc4\_t utf32, st\_uc\_t \* utf8)

utf32 code point to utf8 string


##### Arguments (2)

* `st_uc4_t` *utf32*

* `st_uc_t *` *utf8*


---

<a name="st__zeronull"></a>
### void st\_\_zeronull(char \* str, size\_t bytepos, st\_enc\_t enc)

Add zero null to given position, apropiate for each enc


##### Arguments (3)

* `char *` *str*

* `size_t` *bytepos*

* `st_enc_t` *enc*


---

<a name="st__zeronull_size"></a>
### size\_t st\_\_zeronull\_size(st\_enc\_t enc)

Return the size of zeronull "char" in given `enc`


##### Arguments (1)

* `st_enc_t` *enc*


---

<a name="st_append"></a>
### void st\_append(string \*\* out, string \* src)

Append src to out
[st_concat](#st_concat)
[st_append_char](#st_append_char)
[st_append_c](#st_append_c)



##### Arguments (2)

* `string **` *out*

* `string *` *src*


---

<a name="st_append_c"></a>
### void st\_append\_c(string \*\* out, char \* src)

Append src (c-string) to out
[st_concat](#st_concat)
[st_append](#st_append)
[st_append_char](#st_append_char)



##### Arguments (2)

* `string **` *out*

* `char *` *src*


---

<a name="st_append_char"></a>
### void st\_append\_char(string \*\* out, st\_uc\_t ch)

Append ASCII character at the end of the string.
[st_append](#st_append)
[st_concat](#st_concat)



##### Return: void

new string

##### Arguments (2)

* `string **` *out*

* `st_uc_t` *ch*


---

<a name="st_ascii_char_size"></a>
### st\_len\_t st\_ascii\_char\_size(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_ascii_char_size_safe"></a>
### st\_len\_t st\_ascii\_char\_size\_safe(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_ascii_codepoint"></a>
### st\_uc4\_t st\_ascii\_codepoint(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_ascii_from_codepoint"></a>
### st\_len\_t st\_ascii\_from\_codepoint(char \* out, st\_uc\_t codepoint)



##### Arguments (2)

* `char *` *out*

* `st_uc_t` *codepoint*


---

<a name="st_ascii_lead_size"></a>
### st\_len\_t st\_ascii\_lead\_size(st\_uc\_t lead_chr)



##### Arguments (1)

* `st_uc_t` *lead\_chr*


---

<a name="st_ascii_length"></a>
### st\_len\_t st\_ascii\_length(const char\* src, st\_size\_t \* bytes)



##### Arguments (2)

* `const char*` *src*

* `st_size_t *` *bytes*


---

<a name="st_ascii_valid_codepoint"></a>
### bool st\_ascii\_valid\_codepoint(st\_uc4\_t codepoint)



##### Arguments (1)

* `st_uc4_t` *codepoint*


---

<a name="st_base2number"></a>
### double st\_base2number(string \* src, int base)

Convert a string replesentation of a number in a given base to number.
based on PHP (https://github.com/php/php-src/blob/master/LICENSE)



##### Return: double

number

##### Arguments (2)

* `string *` *src*

* `int` *base*


---

<a name="st_bin2hex"></a>
### string \* st\_bin2hex(const string\* src)

Returns an ASCII string containing the hexadecimal representation of str. The
conversion is done byte-wise with the high-nibble first.

based on PHP (https://github.com/php/php-src/blob/master/LICENSE)

see [st_hex2bin](#st_hex2bin)



##### Return: string \*

new string with the hexadecimal representation of the given string.

##### Arguments (1)

* `const string*` *src*


---

<a name="st_byte_iterator"></a>
### void st\_byte\_iterator(const string\* str, st\_byte\_itr\_cb itr_cb)

Passes each bytes in str to the given function


##### Arguments (2)

* `const string*` *str*

* `st_byte_itr_cb` *itr\_cb*


---

<a name="st_capacity"></a>
### st\_size\_t st\_capacity(const char\* src, st\_enc\_t enc)

Get plain string (null terminated) capacity in given encoding


##### Return: st\_size\_t

string length (do not include null terminated space)

##### Arguments (2)

* `const char*` *src*

* `st_enc_t` *enc*


---

<a name="st_capitalize"></a>
### string \* st\_capitalize(const string\* input)

Returns a copy of `input` with the first character converted to uppercase and
the remainder to lowercase.
see [st_ucfirst](#st_ucfirst)
see [st_lcfirst](#st_lcfirst)



##### Return: string \*

new string

##### Arguments (1)

* `const string*` *input*


---

<a name="st_center"></a>
### string \* st\_center(const string\* src, size\_t width, const string\* padstr)

Center align src and padded with padstr, if width is greater than src length.


##### Return: string \*

new string

##### Arguments (3)

* `const string*` *src*

* `size_t` *width*

* `const string*` *padstr*


---

<a name="st_char_at"></a>
### string \* st\_char\_at(const string\* src, st\_len\_t pos)

Return a string with the char at given position


##### Return: string \*

new string

##### Arguments (2)

* `const string*` *src*

* `st_len_t` *pos*


---

<a name="st_char_iterator"></a>
### void st\_char\_iterator(const string\* str, st\_char\_itr\_cb itr_cb)

Passes each character in str to the given function


##### Arguments (2)

* `const string*` *str*

* `st_char_itr_cb` *itr\_cb*


---

<a name="st_char_map"></a>
### string \* st\_char\_map(const string\* str, st\_char\_map\_cb map_cb)

Create a new string with the results of calling a provided
function on every character in the string.




##### Return: string \*

new string

##### Arguments (2)

* `const string*` *str*

* `st_char_map_cb` *map\_cb*


---

<a name="st_char_size"></a>
### st\_len\_t st\_char\_size(const char\* input, st\_enc\_t enc)

Returns plain string character size (unsafe, no error check)


##### Return: st\_len\_t

1-4 character length

##### Arguments (2)

* `const char*` *input*

* `st_enc_t` *enc*


---

<a name="st_char_size_safe"></a>
### st\_len\_t st\_char\_size\_safe(const char\* input, st\_enc\_t enc)

Returns plain string character size


##### Arguments (2)

* `const char*` *input*

* `st_enc_t` *enc*


---

<a name="st_charmask"></a>
### void st\_charmask(const char\* input, st\_size\_t len, char \* mask)

Create an ascii map


##### Arguments (3)

* `const char*` *input*

* `st_size_t` *len*

* `char *` *mask*


---

<a name="st_chop"></a>
### string \* st\_chop(const string\* str, string \* character_mask)

shortcut to st_trim(x, x, 2)


##### Return: string \*

new string

##### Arguments (2)

* `const string*` *str*

* `string *` *character\_mask*


---

<a name="st_chr"></a>
### string \* st\_chr(st\_uc4\_t value, st\_enc\_t enc)

Returns a new one-character string containing the character specified


##### Return: string \*

new string

##### Arguments (2)

* `st_uc4_t` *value*

* `st_enc_t` *enc*


---

<a name="st_clear"></a>
### void st\_clear(string \* out)

Remove data, do no deallocate anything, just clean.


##### Arguments (1)

* `string *` *out*


---

<a name="st_clone"></a>
### string \* st\_clone(const string\* src)

Return a new string clone of src (same capacity)


##### Return: string \*

new string

##### Arguments (1)

* `const string*` *src*


---

<a name="st_cmp"></a>
### int st\_cmp(const string\* a, const string\* b)

 st_compare


##### Arguments (2)

* `const string*` *a*

* `const string*` *b*


---

<a name="st_codepoint"></a>
### st\_uc4\_t st\_codepoint(const char\* str, st\_enc\_t enc)

Return codepoint in given `enc`


##### Arguments (2)

* `const char*` *str*
@enc encoding
* `st_enc_t` *enc*


---

<a name="st_compare"></a>
### int st\_compare(const string\* a, const string\* b)

Compare two strings byte-to-byte.


##### Return: int

* <0 a < b (length or content)
* =0 Both strings has same content & length.
* \>0 a > b (length or content)

##### Arguments (2)

* `const string*` *a*

* `const string*` *b*


---

<a name="st_concat"></a>
### string \* st\_concat(string \* first, string \* second)

Concatenate two strings and return a new one.
[st_append](#st_append)
[st_append_char](#st_append_char)



##### Return: string \*

new string

##### Arguments (2)

* `string *` *first*

* `string *` *second*


---

<a name="st_concat_random"></a>
### string \* st\_concat\_random(string \* first, st\_len\_t len)

Concatenate two strings (second random) and return a new one.
[st_append](#st_append)
[st_concat](#st_concat)
[st_append_char](#st_append_char)



##### Return: string \*

new string

##### Arguments (2)

* `string *` *first*

* `st_len_t` *len*


---

<a name="st_contains"></a>
### bool st\_contains(const string\* haystack, const string\* needle)

Shortcut of [st_pos](#st_pos) that only check if it's found


##### Return: bool

true if found, false otherwise

##### Arguments (2)

* `const string*` *haystack*

* `const string*` *needle*


---

<a name="st_copy"></a>
### void st\_copy(string \*\* out, const string\* src)

Copy src into out
> out will be resized if needed



##### Arguments (2)

* `string **` *out*

* `const string*` *src*


---

<a name="st_copyc"></a>
### void st\_copyc(string \*\* out, const char\* src, st\_enc\_t enc)

Copy src into out
> out will be resized if needed



##### Arguments (3)

* `string **` *out*

* `const char*` *src*

* `st_enc_t` *enc*


---

<a name="st_cspn"></a>
### st\_len\_t st\_cspn(const string\* subject, const string\* mask, st\_len\_t offset, st\_len\_t length)

Return the position of the maximum initial segment of S
of `subject` which contains no characters in `mask`.



##### Return: st\_len\_t

position

##### Arguments (4)

* `const string*` *subject*

* `const string*` *mask*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_debug"></a>
### void st\_debug(string \* str)

Print to stdout useful information to debug
It add zeronull at the end of the string to not overflow when printf



##### Arguments (1)

* `string *` *str*


---

<a name="st_delete"></a>
### void st\_delete(string \*\* out)

Free given string
> `out` will be modified and pointed to 0 (no dangling)
>
> To edit deallocator define: __STRING_DEALLOCATOR



##### Arguments (1)

* `string **` *out*


---

<a name="st_dump"></a>
### char \* st\_dump(string \* s)

wip


##### Arguments (1)

* `string *` *s*


---

<a name="st_end_with"></a>
### bool st\_end\_with(const string\* haystack, const string\* needle)

Returns if haystack ends with needle


##### Return: bool

true if ends, false otherwise

##### Arguments (2)

* `const string*` *haystack*

* `const string*` *needle*


---

<a name="st_escape"></a>
### string \* st\_escape(const string\* str)

Escape string. Transform newline -> '\n'


##### Arguments (1)

* `const string*` *str*


---

<a name="st_from_codepoint"></a>
### st\_len\_t st\_from\_codepoint(char \* out, st\_uc4\_t codepoint, st\_enc\_t enc)

Return length of `codepoint` stored in `out` in given encoding.


##### Return: st\_len\_t

length of given encoding

##### Arguments (3)

* `char *` *out*

* `st_uc4_t` *codepoint*

* `st_enc_t` *enc*


---

<a name="st_get_meta"></a>
### void st\_get\_meta(const char\* src, st\_enc\_t enc, st\_len\_t \* len, st\_size\_t \* capacity)

Get capacity and length of given string in given encoding


##### Arguments (4)

* `const char*` *src*

* `st_enc_t` *enc*

* `st_len_t *` *len*

* `st_size_t *` *capacity*


---

<a name="st_grow"></a>
### void st\_grow(string \*\* out, st\_size\_t cap, st\_enc\_t enc)

Grow given string.
If string point to 0 -> st_new
if string has less capacity that needed -> st_resize




##### Arguments (3)

* `string **` *out*

* `st_size_t` *cap*

* `st_enc_t` *enc*


---

<a name="st_hex2bin"></a>
### string \* st\_hex2bin(string \* src)

Returns a new string result of decoding a hexadecimally encoded
binary string

based on PHP (https://github.com/php/php-src/blob/master/LICENSE)



##### Return: string \*

new string

##### Arguments (1)

* `string *` *src*


---

<a name="st_hexdump"></a>
### void st\_hexdump(const char\* p, st\_size\_t size)

Print to stdout hex data for given plain string


##### Arguments (2)

* `const char*` *p*

* `st_size_t` *size*


---

<a name="st_index_of"></a>
### st\_len\_t st\_index\_of(const string\* haystack, const string\* needle, st\_len\_t offset, st\_len\_t length)

alias of [st_pos](#st_pos)


##### Return: st\_len\_t

* \> 0 position of the first occurrence if found
* -1 if not found

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_insert"></a>
### void st\_insert(string \*\* out, const string\* ins, st\_len\_t offset)

Add `ins` into `offset` position


##### Return: void

position

##### Arguments (3)

* `string **` *out*

* `const string*` *ins*

* `st_len_t` *offset*


---

<a name="st_ipos"></a>
### st\_len\_t st\_ipos(const string\* haystack, const string\* needle, st\_len\_t offset, st\_len\_t length)

returns the position of the last occurrence of a specified value in a string.


##### Return: st\_len\_t

* -1 if not found
* \> -1 if found, position

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_irpos"></a>
### st\_len\_t st\_irpos(const string\* haystack, const string\* needle, st\_len\_t offset, st\_len\_t length)

[st_pos](#st_pos) but case insensitive


##### Return: st\_len\_t

new string

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_is_ascii"></a>
### bool st\_is\_ascii(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_justify"></a>
### string \* st\_justify(const string\* src, st\_len\_t width, const string\* padstr, char mode)

Justify src and padded with padstr, if width is greater than src length.


##### Return: string \*

new string

##### Arguments (4)

* `const string*` *src*

* `st_len_t` *width*

* `const string*` *padstr*

* `char` *mode*


---

<a name="st_lcfirst"></a>
### string \* st\_lcfirst(string \* str)

Returns a copy of `input` with the first character lowercased.
see [st_capitalize](#st_capitalize)
see [st_ucfirst](#st_ucfirst)



##### Return: string \*

new string

##### Arguments (1)

* `string *` *str*


---

<a name="st_left"></a>
### string \* st\_left(const string\* src, size\_t width, const string\* padstr)

Left align src and padded with padstr, if width is greater than src length.


##### Return: string \*

new string

##### Arguments (3)

* `const string*` *src*

* `size_t` *width*

* `const string*` *padstr*


---

<a name="st_length"></a>
### st\_len\_t st\_length(const char\* src, st\_enc\_t enc)

Get plain string (null terminated) length in given encoding


##### Return: st\_len\_t

string length

##### Arguments (2)

* `const char*` *src*

* `st_enc_t` *enc*


---

<a name="st_line_iterator"></a>
### void st\_line\_iterator(const string\* str, st\_char\_itr\_cb itr_cb)

Split given string by newline and passes each substring (newline won't be
included).



##### Arguments (2)

* `const string*` *str*

* `st_char_itr_cb` *itr\_cb*


---

<a name="st_lower"></a>
### string \* st\_lower(const string\* input)

Make a string lowercase


##### Return: string \*

new string

##### Arguments (1)

* `const string*` *input*


---

<a name="st_ltrim"></a>
### string \* st\_ltrim(const string\* str, string \* character_mask)

shortcut to st_trim(x, x, 2)


##### Return: string \*

new string

##### Arguments (2)

* `const string*` *str*

* `string *` *character\_mask*


---

<a name="st_memfree"></a>
### void st\_memfree

Clean up operation, call this before exit.
Some operation require intermediate/costly objects
st_memfree take care of them and free that memory
> can be called more than once to free memory


---

<a name="st_new"></a>
### string \* st\_new(st\_size\_t cap, st\_enc\_t enc)

Allocate a new empty string
> To edit allocator define: __STRING_ALLOCATOR



##### Return: string \*

new string

##### Arguments (2)

* `st_size_t` *cap*

* `st_enc_t` *enc*


---

<a name="st_new_max"></a>
### string \* st\_new\_max(st\_len\_t len, st\_enc\_t enc)

Allocate a new empty with the maximum capacity needed to store given len
> To edit allocator define: __STRING_ALLOCATOR



##### Return: string \*

new string

##### Arguments (2)

* `st_len_t` *len*

* `st_enc_t` *enc*


---

<a name="st_new_subc"></a>
### string \* st\_new\_subc(const char\* src, st\_size\_t len, st\_enc\_t enc)

Create a new string from given substring


##### Return: string \*

new string

##### Arguments (3)

* `const char*` *src*

* `st_size_t` *len*

* `st_enc_t` *enc*


---

<a name="st_newc"></a>
### string \* st\_newc(const char\* src, st\_enc\_t enc)

Allocate a new string and copy src into it.
The string capacity will be the minimal necessary to store src
> To edit allocator define: __STRING_ALLOCATOR



##### Return: string \*

new string

##### Arguments (2)

* `const char*` *src*

* `st_enc_t` *enc*


---

<a name="st_number2base"></a>
### string \* st\_number2base(size\_t value, int base)

Returns a new string containing the representation of the given (unsigned)
number argument in given base.

based on PHP (https://github.com/php/php-src/blob/master/LICENSE)



##### Return: string \*

new string

##### Arguments (2)

* `size_t` *value*

* `int` *base*


---

<a name="st_ord"></a>
### st\_uc4\_t st\_ord(const string\* str, st\_len\_t offset)

Return the code point at specified offset


##### Return: st\_uc4\_t

code point

##### Arguments (2)

* `const string*` *str*

* `st_len_t` *offset*


---

<a name="st_pos"></a>
### st\_len\_t st\_pos(const string\* haystack, const string\* needle, st\_len\_t offset, st\_len\_t length)

Find the numeric position of the first occurrence of needle in the haystack
string.
Returns -1 if the value to search never occurs.
[st__calc_range](#st__calc_range) to see how offset & length works



##### Return: st\_len\_t

* \> 0 position of the first occurrence if found
* -1 if not found

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_rclone"></a>
### string \* st\_rclone(const string\* src, st\_size\_t cap)

Return a new string clone of src with given capacity (resize)


##### Return: string \*

new string

##### Arguments (2)

* `const string*` *src*

* `st_size_t` *cap*


---

<a name="st_remove"></a>
### string \* st\_remove(const string\* haystack, const string\* needle, st\_len\_t offset, st\_len\_t length)

Remove any occurrence of needle in haystack


##### Return: string \*

new string

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_repeat"></a>
### string \* st\_repeat(const string\* src, size\_t times)

Returns a new string result of repeating src x times.
based on PHP (https://github.com/php/php-src/blob/master/LICENSE)



##### Return: string \*

new string

##### Arguments (2)

* `const string*` *src*

* `size_t` *times*


---

<a name="st_replace"></a>
### string \* st\_replace(const string\* haystack, const string\* needle, const string\* replacement, st\_len\_t \* count)

Replace all occurrences of the search string with the replacement string
Caveats
It will replace left to right always reading haystack.
That means replacement wont be included in needle test ever, avoid possible
infinite loop.
st_replace("abcd", "bc", "abc")



##### Return: string \*

new string

##### Arguments (4)

* `const string*` *haystack*

* `const string*` *needle*

* `const string*` *replacement*

* `st_len_t *` *count*


---

<a name="st_replace_allocators"></a>
### void st\_replace\_allocators(st\_malloc\_func malloc_func, st\_realloc\_func realloc_func, st\_free\_func free_func)

Replace built-in allocators: malloc, realloc & free
null won't modify current allocators, if you want to reset
call `st_replace_allocators(malloc, realloc, free)` instead.


##### Arguments (3)

* `st_malloc_func` *malloc\_func*

* `st_realloc_func` *realloc\_func*

* `st_free_func` *free\_func*


---

<a name="st_resize"></a>
### void st\_resize(string \*\* src, st\_size\_t cap)

Reallocate src with given len
> to edit allocator define: __STRING_REALLOCATOR



##### Return: void

new string

##### Arguments (2)

* `string **` *src*

* `st_size_t` *cap*


---

<a name="st_right"></a>
### string \* st\_right(const string\* src, size\_t width, const string\* padstr)

Right align src and padded with padstr, if width is greater than src length.


##### Return: string \*

new string

##### Arguments (3)

* `const string*` *src*

* `size_t` *width*

* `const string*` *padstr*


---

<a name="st_rpos"></a>
### st\_len\_t st\_rpos(string \* haystack, string \* needle, st\_len\_t offset, st\_len\_t len)



##### Arguments (4)

* `string *` *haystack*

* `string *` *needle*

* `st_len_t` *offset*

* `st_len_t` *len*


---

<a name="st_rtrim"></a>
### string \* st\_rtrim(const string\* str, string \* character_mask)

shortcut to st_trim(x, x, 2)


##### Return: string \*

new string

##### Arguments (2)

* `const string*` *str*

* `string *` *character\_mask*


---

<a name="st_scmp"></a>
### int st\_scmp(const string\* a, const string\* b, st\_len\_t offset, st\_len\_t length)

 st_compare


##### Arguments (4)

* `const string*` *a*

* `const string*` *b*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_shuffle"></a>
### string \* st\_shuffle(string \* src, size\_t len)

Shuffle (randomize) given string


##### Return: string \*

new string

##### Arguments (2)

* `string *` *src*

* `size_t` *len*


---

<a name="st_spn"></a>
### st\_len\_t st\_spn(const string\* subject, const string\* mask, st\_len\_t offset, st\_len\_t length)

Return the position of the maximum initial segment
of `subject` which contains only characters in `mask`.



##### Return: st\_len\_t

position

##### Arguments (4)

* `const string*` *subject*

* `const string*` *mask*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_start_with"></a>
### bool st\_start\_with(const string\* haystack, const string\* needle)

Returns if haystack starts with needle


##### Return: bool

true if start, false otherwise

##### Arguments (2)

* `const string*` *haystack*

* `const string*` *needle*


---

<a name="st_sub"></a>
### string \* st\_sub(const string\* str, int start, int end)

Extracts the characters from a string, between two specified indices, and
returns the new sub string.
TODO review and do some comparison with PHP/Javascript usage



##### Return: string \*

new string

##### Arguments (3)

* `const string*` *str*

* `int` *start*

* `int` *end*


---

<a name="st_subcompare"></a>
### int st\_subcompare(const string\* a, const string\* b, st\_len\_t offset, st\_len\_t length)

Compare a substring against b string
[st__calc_range](#st__calc_range)



##### Return: int

abc
* <0 a < b (length or content)
* =0 Both strings has same content & length.
* >0 a > b (length or content)

##### Arguments (4)

* `const string*` *a*

* `const string*` *b*

* `st_len_t` *offset*

* `st_len_t` *length*


---

<a name="st_to_utf32"></a>
### string \* st\_to\_utf32(const string\* src)

Enconde a string into utf32


##### Arguments (1)

* `const string*` *src*


---

<a name="st_to_utf8"></a>
### string \* st\_to\_utf8(const string\* src)

Enconde a string into utf8


##### Arguments (1)

* `const string*` *src*


---

<a name="st_trim"></a>
### string \* st\_trim(const string\* str, string \* character_mask, int mode)

Strip whitespace (or given mask) from the beginning and/or end of a string


##### Return: string \*

new string

##### Arguments (3)

* `const string*` *str*

* `string *` *character\_mask*

* `int` *mode*


---

<a name="st_ucfirst"></a>
### string \* st\_ucfirst(const string\* input)

Returns a copy of `input` with the first character uppercased.
see [st_capitalize](#st_capitalize)
see [st_lcfirst](#st_lcfirst)



##### Return: string \*

new string

##### Arguments (1)

* `const string*` *input*


---

<a name="st_unescape"></a>
### string \* st\_unescape(const string\* str)

Unescape string. Transform '\n' -> newline


##### Arguments (1)

* `const string*` *str*


---

<a name="st_upper"></a>
### string \* st\_upper(const string\* input)

Make a string uppercase


##### Return: string \*

new string

##### Arguments (1)

* `const string*` *input*


---

<a name="st_utf32_char_size"></a>
### st\_len\_t st\_utf32\_char\_size(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32_lead_size"></a>
### st\_len\_t st\_utf32\_lead\_size(st\_uc\_t lead_chr)



##### Arguments (1)

* `st_uc_t` *lead\_chr*


---

<a name="st_utf32_length"></a>
### st\_len\_t st\_utf32\_length(const char\* src, st\_size\_t \* bytes)



##### Arguments (2)

* `const char*` *src*

* `st_size_t *` *bytes*


---

<a name="st_utf32_lowercase"></a>
### st\_uc4\_t st\_utf32\_lowercase(st\_uc4\_t utf32)

Lowercase unicode codepoint


##### Return: st\_uc4\_t

lowercased unicode codepoint

##### Arguments (1)

* `st_uc4_t` *utf32*


---

<a name="st_utf32_uppercase"></a>
### st\_uc4\_t st\_utf32\_uppercase(st\_uc4\_t utf32)

Uppercase unicode codepoint


##### Return: st\_uc4\_t

uppercased unicode codepoint

##### Arguments (1)

* `st_uc4_t` *utf32*


---

<a name="st_utf32_valid_codepoint"></a>
### bool st\_utf32\_valid\_codepoint(st\_uc4\_t codepoint)



##### Arguments (1)

* `st_uc4_t` *codepoint*


---

<a name="st_utf32be_char_size"></a>
### st\_len\_t st\_utf32be\_char\_size(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32be_char_size_safe"></a>
### st\_len\_t st\_utf32be\_char\_size\_safe(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32be_codepoint"></a>
### st\_uc4\_t st\_utf32be\_codepoint(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32be_from_codepoint"></a>
### st\_len\_t st\_utf32be\_from\_codepoint(const char\* input, st\_uc4\_t codepoint)



##### Arguments (2)

* `const char*` *input*

* `st_uc4_t` *codepoint*


---

<a name="st_utf32le_char_size"></a>
### st\_len\_t st\_utf32le\_char\_size(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32le_char_size_safe"></a>
### st\_len\_t st\_utf32le\_char\_size\_safe(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32le_codepoint"></a>
### st\_uc4\_t st\_utf32le\_codepoint(const char\* input)



##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf32le_from_codepoint"></a>
### st\_len\_t st\_utf32le\_from\_codepoint(const char\* input, st\_uc4\_t codepoint)



##### Arguments (2)

* `const char*` *input*

* `st_uc4_t` *codepoint*


---

<a name="st_utf8_char_eq"></a>
### bool st\_utf8\_char\_eq(char \* a, char \* b)

Returns if two utf8 are the same. Comparison is multibyte.


##### Return: bool

true if are the same

##### Arguments (2)

* `char *` *a*

* `char *` *b*


---

<a name="st_utf8_char_size"></a>
### st\_len\_t st\_utf8\_char\_size(const char\* input)

Return how many bytes contains given lead, with no error control.


##### Return: st\_len\_t

* 1-4 if success

##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf8_char_size_safe"></a>
### st\_len\_t st\_utf8\_char\_size\_safe(const char\* input)

Return how many bytes contains given lead and -1 if it's invalid.


##### Return: st\_len\_t

* -1 on error
* 1-4 if success

##### Arguments (1)

* `const char*` *input*


---

<a name="st_utf8_codepoint"></a>
### st\_uc4\_t st\_utf8\_codepoint(const char\* utf8)

utf8 string to utf32 code point


##### Return: st\_uc4\_t

codepoint

##### Arguments (1)

* `const char*` *utf8*


---

<a name="st_utf8_from_codepoint"></a>
### st\_len\_t st\_utf8\_from\_codepoint(char \* utf8, st\_uc4\_t codepoint)

parse codepoint into utf8 (NOT null terminated)


##### Return: st\_len\_t

bytes used by given codepoint

##### Arguments (2)

* `char *` *utf8*

* `st_uc4_t` *codepoint*


---

<a name="st_utf8_invalid"></a>
### char \* st\_utf8\_invalid(const st\_uc\_t\* str, size\_t len)

Check if the given uchar* is a valid utf-8 sequence.
Return value :
If the string is valid utf-8, 0 is returned.
Else the position, starting from 1, is returned.

Valid utf-8 sequences look like this :

0xxxxxxx

110xxxxx 10xxxxxx

1110xxxx 10xxxxxx 10xxxxxx

11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

Based on is_utf8 by JulienPalard, heavily modified by llafuente
to archieve great performance.

based on PHP (https://github.com/php/php-src/blob/master/LICENSE)



##### Return: char \*

first invalid position found or 0 if success

##### Arguments (2)

* `const st_uc_t*` *str*

* `size_t` *len*


---

<a name="st_utf8_lead_size"></a>
### st\_len\_t st\_utf8\_lead\_size(st\_uc\_t lead)

Return how many bytes contains given lead, with no error control.


##### Return: st\_len\_t

* 1-4 if success

##### Arguments (1)

* `st_uc_t` *lead*


---

<a name="st_utf8_length"></a>
### st\_len\_t st\_utf8\_length(const char\* src, st\_size\_t \* bytes)

Return utf8 length and bytes
based on glib_utf8_offset_to_pointer



##### Return: st\_len\_t

string length utf-8 encoded

##### Arguments (2)

* `const char*` *src*

* `st_size_t *` *bytes*


---

<a name="st_utf8_valid_codepoint"></a>
### bool st\_utf8\_valid\_codepoint(st\_uc4\_t cp)

Returns if the given unicode code-point is valid


##### Return: bool

true if valid, false otherwise

##### Arguments (1)

* `st_uc4_t` *cp*


---

<a name="st_validate_encoding"></a>
### bool st\_validate\_encoding(char \* input, st\_enc\_t enc)

Validate input string.


##### Return: bool

true if is valid
false if not

##### Arguments (2)

* `char *` *input*

* `st_enc_t` *enc*


---

<a name="string_encode"></a>
### string \* string\_encode(string \* src, st\_enc\_t to_enc)

> TODO encode a string to given encoding


##### Return: string \*

new string

##### Arguments (2)

* `string *` *src*

* `st_enc_t` *to\_enc*


---

