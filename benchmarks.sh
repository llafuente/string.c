#!/bin/sh

#check argument, print usage

mkdir build
cd build

make clean
make distclean
sh ../autogen.sh
../configure

make check "CC='clang'" "CFLAGS=-O3 -DBENCHMARKING"
cd ..

mkdir bench_data
build/test/run-tests $1
