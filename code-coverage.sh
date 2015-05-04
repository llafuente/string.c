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
# -v
gcovr -r .. --html --html-details -o code-coverage/index.html \
--exclude 'src/picobase\.c' \
--exclude 'test/run-tests\.c' \
--exclude 'test/tasks\.c' \
--exclude 'test/test-alloc\.c' \
--exclude 'test/test-append\.c' \
--exclude 'test/test-ascii\.c' \
--exclude 'test/test-case\.c' \
--exclude 'test/test-compare\.c' \
--exclude 'test/test-encoding\.c' \
--exclude 'test/test-from\.c' \
--exclude 'test/test-hexbinhex\.c' \
--exclude 'test/test-internal\.c' \
--exclude 'test/test-iterators\.c' \
--exclude 'test/test-justify\.c' \
--exclude 'test/test-repeat\.c' \
--exclude 'test/test-search\.c' \
--exclude 'test/test-shuffle\.c' \
--exclude 'test/test-sub\.c' \
--exclude 'test/test-to\.c' \
--exclude 'test/test-trim\.c' \
--exclude 'test/test-utf8\.c' \
--exclude 'test/test-utils\.c'



cd ..
