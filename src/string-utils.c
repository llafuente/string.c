#include "stringc.h"

void charmask(unsigned char *input, size_t len, char *mask) {
  unsigned char *end;
  memset(mask, 0, 256);
  for (end = input+len; input < end; ++input) {
    mask[*input] = 1;
  }
}
