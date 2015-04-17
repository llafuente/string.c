#!/bin/sh -x

mkdir build

cd build

make clean
rm -rf *.gcno *.gcda *.gcov

sh ../autogen.sh
../configure
make check "CFLAGS=--coverage"

rm -rf code-coverage
mkdir code-coverage
gcovr -r .. --html --html-details -o code-coverage/index.html -v

cd ..
