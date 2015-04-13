# string.c

Single allocation string implementation (with struct) c99 at least,
more convinient and performance that common null terminated char*,
100% compatible with them :) and accept multiple encodings: ascii, utf8 &
ucs4be.

It's not yet ready to be a shared library, use static instead. It's not stable
enough.

## Usage

include a single header

```c
#include "stringc.h"
```

include .libs/ and link against libstringc.la


## Compilation

autoconf is required.

RHEL
```bash
sudo yum install autoconf
sudo yum install libtool
```

```bash
sh autogen.sh
./configure
make
```

## Tests

```bash
make check
```

## Style / format

We use clang-format
To keep coding style sanity, before PR use:

```bash
format.sh
```
