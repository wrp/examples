
include_HEADERS = hashmap.h
check_PROGRAMS = hash-test builtin-test
noinst_PROGRAMS = benchmark
TESTS = hash-test builtin-test coverage-test

lib_LTLIBRARIES = libhash.la
LDADD=libhash.la
libhash_la_SOURCES = hashmap.c

hash_test_SOURCES = hashtest.c

bench: benchmark
	./benchmark

CLEANFILES = *.gcda *.gcno *.gcov *.o
