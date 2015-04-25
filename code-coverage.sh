#!/bin/sh -x

mkdir build

cd build

make clean
make distclean
rm -rf *.gcno *.gcda *.gcov

sh ../autogen.sh
../configure
make check "CFLAGS=-fprofile-arcs -ftest-coverage"

rm -rf code-coverage
mkdir code-coverage
gcovr -r .. --html --html-details -o code-coverage/index.html -v --exclude 'src/picobase\.c'

cd ..
