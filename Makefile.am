## TODO add `rm -f *.gcno *.gcda *.gcov` to clean


ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/src

include_HEADERS=include/stringc.h

CLEANFILES =

lib_LTLIBRARIES = libstringc.la
libstringc_la_CFLAGS =
libstringc_la_LDFLAGS = -no-undefined -version-info 1:0:0
libstringc_la_SOURCES = src/stringc.c \
                  src/append.c \
                  src/alloc.c \
                  src/ascii.c \
                  src/bin2hex.c \
                  src/case.c \
                  src/compare.c \
                  src/encode.c \
                  src/to.c \
                  src/hex2bin.c \
                  src/internal.c \
                  src/justify.c \
                  src/iterators.c \
                  src/picobase.c \
                  src/repeat.c \
                  src/search.c \
                  src/shuffle.c \
                  src/substring.c \
                  src/trim.c \
                  src/utf8.c \
                  src/utf32.c \
                  src/utils.c

SOURCES_TEST =           test/tasks.c \
                         test/test-alloc.c \
                         test/test-append.c \
                         test/test-ascii.c \
                         test/test-case.c \
                         test/test-compare.c \
                         test/test-customalloc.c \
                         test/test-encoding.c \
                         test/test-from.c \
                         test/test-hexbinhex.c \
                         test/test-internal.c \
                         test/test-iterators.c \
                         test/test-justify.c \
                         test/test-repeat.c \
                         test/test-search.c \
                         test/test-shuffle.c \
                         test/test-sub.c \
                         test/test-to.c \
                         test/test-trim.c \
                         test/test-utf32.c \
                         test/test-utf8.c \
                         test/test-utils.c


TESTS = test/run-tests
check_PROGRAMS = test/run-tests
test_run_tests_CFLAGS = -O1
test_run_tests_LDFLAGS = -O1 -lrt
test_run_tests_SOURCES = test/run-tests.c
#test_run_tests_SOURCES = test/run-benchmarks.c
test_run_tests_SOURCES += ${SOURCES_TEST}

test_run_tests_LDADD = libstringc.la
