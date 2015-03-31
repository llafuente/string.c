#include "string-shuffle.h"

string* string_shuffle(string* src, size_t len) {
  int rnd_idx;
  char temp;
  /* The implementation is stolen from array_data_shuffle */
  /* Thus the characteristics of the randomization are the same */
  string* out = string_clone(src);

  if (len <= 1) {
    return out;
  }

  char* str = out->value;

  while (--len) {
    rnd_idx = (int) (len * (rand() / (RAND_MAX + 1.0)));

    if (rnd_idx != len) {
      temp = str[len];
      str[len] = str[rnd_idx];
      str[rnd_idx] = temp;
    }
  }

  return out;
}
