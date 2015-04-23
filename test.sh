#!/bin/sh

mkdir build
cd build

ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer)
ASAN_OPTIONS=symbolize=1

make clean
make distclean
sh ../autogen.sh
../configure
#-fsanitize-memory could be needed ?
make check "CC='clang'" "CFLAGS=-g -O0 -fsanitize=integer -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -fsanitize-memory-track-origins"

cd ..
