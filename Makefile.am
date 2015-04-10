CC = clang

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/src

include_HEADERS=include/stringc.h

CLEANFILES =

lib_LTLIBRARIES = libstringc.la
libstringc_la_CFLAGS = @CFLAGS@
libstringc_la_LDFLAGS = -no-undefined -version-info 1:0:0
libstringc_la_SOURCES = src/append.c \
                  src/alloc.c \
                  src/bin2hex.c \
                  src/case.c \
                  src/compare.c \
                  src/to.c \
                  src/hex2bin.c \
                  src/internal.c \
                  src/iterators.c \
                  src/repeat.c \
                  src/search.c \
                  src/shuffle.c \
                  src/substring.c \
                  src/trim.c \
                  src/utf8.c \
                  src/utils.c


TESTS = test/run-tests
check_PROGRAMS = test/run-tests
test_run_tests_CFLAGS = -fsanitize=address -fno-omit-frame-pointer -O1
test_run_tests_LDFLAGS = -fsanitize=address -fno-omit-frame-pointer -O1
test_run_tests_SOURCES = test/test.c
test_run_tests_LDADD = libstringc.la
