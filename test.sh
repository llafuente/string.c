#!/bin/sh

reset

rm string_test_mem
rm string_test_c

#echo
#echo "--- LLVM IR --"
#echo
#clang -S -emit-llvm -O0 -std=c11 -Wgcc-compat -Wall -v test/test.c - o llvm-ir &> /dev/null || exit 1
echo "--- COMPILE (CPP) ---"
## ?? -I./src -I./include
## -Wextra -pedantic
clang -O0 -g -fsanitize=address -fno-omit-frame-pointer -std=c++11  -Wgcc-compat -Wall test/test.cpp -o string_test_mem &> /dev/null || exit 1
sleep 1

echo "--- TEST (with sanitize-address) --- "
# disable valgrind, AddressSanitizer seems better
#valgrind --track-origins=yes --leak-check=full -v ./string
#echo ""
#echo "----"
#echo ""
./string_test_mem

echo "--- COMPILE (C) ---"
# test pure c
cp test/test.cpp test/test.c
clang -O0 -std=c11 -Wgcc-compat -Wall test/test.c -o string_test_c &> /dev/null || exit 1
sleep 1

echo "--- TEST (C) --- "
./string_test_c
