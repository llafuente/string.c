#include "string-trim.h"
#include "stringc.h"

string* string_trim(string *str, string *what_str, int mode) {
  const char *c = str->value;
  size_t len = str->length;
  size_t i;
  size_t trimmed = 0;

  if (!what_str) {
    if (!string_def_trim_mask) {
      string_def_trim_mask = string_newc(" \t\n\r\0\x0B", ascii);
    }
    what_str = string_def_trim_mask;
  }
  const char *what = what_str->value;
  size_t what_len = what_str->length;

  if (what_len == 1) {
    char whatc = *what;
    if (mode & 1) {
      for (i = 0; i < len; i++) {
        if (c[i] == whatc) {
          trimmed++;
        } else {
          break;
        }
      }
      len -= trimmed;
      c += trimmed;
    }
    if (mode & 2) {
      if (len > 0) {
        i = len - 1;
        do {
          if (c[i] == whatc) {
            len--;
          } else {
            break;
          }
        } while (i-- != 0);
      }
    }
  } else {
    char mask[256];
    charmask((unsigned char*)what, what_len, mask);
    if (mode & 1) {
      for (i = 0; i < len; i++) {
        if (mask[(unsigned char)c[i]]) {
          trimmed++;
        } else {
          break;
        }
      }
      len -= trimmed;
      c += trimmed;
    }
    if (mode & 2) {
      if (len > 0) {
        i = len - 1;
        do {
          if (mask[(unsigned char)c[i]]) {
            len--;
          } else {
            break;
          }
        } while (i-- != 0);
      }
    }
  }

  if (str->length == len) {
    return string_clone(str);
  }

  return string_clonec(c, len);
}
