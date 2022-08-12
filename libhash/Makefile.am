
include_HEADERS = hashmap.h
check_PROGRAMS = hash-test builtin-test
TESTS = hash-test builtin-test coverage-test

lib_LTLIBRARIES = libhash.la
LDADD=libhash.la
libhash_la_SOURCES = hashmap.c

hash_test_SOURCES = hashtest.c

builtin_test_CPPFLAGS = -DHASHMAP_TEST
builtin_test_SOURCES = hashmap.c

CLEANFILES = *.gcda *.gcno *.gcov *.o
