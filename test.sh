#!/bin/sh

mkdir build
cd build

make clean
sh ../autogen.sh
../configure
make check "CC='clang'" "CFLAGS=-fsanitize=address -fno-omit-frame-pointer"

cd ..
