#!/bin/sh

reset

#-Wextra -pedantic
clang -O3 -std=gnu11 -Wgcc-compat -Wall -lrt -v bench/benchmarks.c -o string_bench || exit 1

#echo -e "\n\nexec \n"
# ./string_bench
