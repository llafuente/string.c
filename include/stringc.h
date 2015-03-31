// common headers
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string-type.h"
#include "string-utils.h"

// some caches
static string* string_def_trim_mask = 0;

void string_zeronull(string* str);

/**
 * print to stdout useful information to debug
 */
void string_debug(string* str);
