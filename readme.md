# string.c

Single allocation string implementation (with struct) c99 at least,
more convenient and performance that common null terminated char*,
100% compatible with them :) and accept multiple encodings.

It's not yet ready to be a shared library, use static instead. It's not stable
enough.

## Encodings

* UTF8
* UTF32be
* UTF32le
* ascii
* binary

  Used for unknown or user data. Internally is handled as ascii (byte to byte)

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


## Code-coverage

```bash
sh autogen.sh && ./configure && make check "CFLAGS='--coverage'"
rm -rf code-coverage
mkdir code-coverage
gcovr -r . --html --html-details -o code-coverage/index.html
```


# install pip & gcovr

install phyton...

```bash
cd /tmp
wget https://bootstrap.pypa.io/get-pip.py
sudo python get-pip.py
sudo pip install gcovr
```
