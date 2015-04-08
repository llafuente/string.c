CC = clang

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/src

include_HEADERS=include/stringc.h

CLEANFILES =

lib_LTLIBRARIES = libstringc.la
libstringc_la_CFLAGS = @CFLAGS@
libstringc_la_LDFLAGS = -no-undefined -version-info 1:0:0
libstringc_la_SOURCES = src/stringc.c \
                  src/string-append.c \
                  src/string-bin2hex.c \
                  src/string-case.c \
                  src/string-compare.c \
                  src/string-from.c \
                  src/string-hex2bin.c \
                  src/string-iterator.c \
                  src/string-memory.c \
                  src/string-repeat.c \
                  src/string-shuffle.c \
                  src/string-sub.c \
                  src/string-trim.c \
                  src/string-utf8.c \
                  src/utils.c


TESTS = test/run-tests
check_PROGRAMS = test/run-tests
test_run_tests_CFLAGS = -fsanitize=address -fno-omit-frame-pointer -O1
test_run_tests_LDFLAGS = -fsanitize=address -fno-omit-frame-pointer -O1
test_run_tests_SOURCES = test/test.c
test_run_tests_LDADD = libstringc.la
