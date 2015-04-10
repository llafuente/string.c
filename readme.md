# string.c

Single allocation string implementation, more convinient and performance
that null terminated chars and 100% compatible with them :)

For performance reason, atm, is not a shared library.

## Usage

include a single header

```c
#include "stringc.h"
```

include .libs/ and link against libstringc.la
