#include "string-from.h"

/**
* bindec, 2
* octdec, 8
* hexdec, 16
*/
double string_from_base(string *str, int base) {
  size_t num = 0;
  double fnum = 0;
  size_t i;
  int mode = 0;
  char c, *s = str->value;
  size_t cutoff;
  int cutlim;

  cutoff = INT64_MAX / base;
  cutlim = INT64_MAX % base;
  for (i = str->length; i > 0; i--) {
    c = *s++;
    /* might not work for EBCDIC */
    if (c >= '0' && c <= '9') {
      c -= '0';
    } else if (c >= 'A' && c <= 'Z') {
      c -= 'A' - 10;
    } else if (c >= 'a' && c <= 'z') {
      c -= 'a' - 10;
    } else {
      continue;
    }

    if (c >= base) {
      continue;
    }

    switch (mode) {
      case 0: /* Integer */
      if (num < cutoff || (num == cutoff && c <= cutlim)) {
        num = num * base + c;
        break;
      } else {
        fnum = (double)num;
        mode = 1;
      }
      /* fall-through */
      case 1: /* Float */
      fnum = fnum * base + c;
    }
  }

  if (mode == 1) {
    return fnum;
  }

  return num;
}


/*
* Convert a long to a string containing a base(2-36) representation of
* the number.
* decbin, 2
* decoct, 8
* dechex, 16
*/
string* string_from_number(size_t value, int base) {
  static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  string* result;
  char *ptr;
  char *end;
  char buf[(sizeof(double) << 3) + 1]; // maximum size for binary

  end = ptr = buf + sizeof(buf) - 1;
  *ptr = '\0';
  // reverse buffer filling data, ptr will be char* containing data
  do {
    *--ptr = digits[value % base];
    value /= base;
  } while (ptr > buf && value);
  //?? *end = '\0';
  // size
  result = string_new(end - ptr);
  string_copyc(&result, ptr);
  //result->length = end - ptr;
  return result;
}
