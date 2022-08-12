
include_HEADERS = hashmap.h
check_PROGRAMS = hash-test builtin-test
noinst_PROGRAMS = hash-bench
TESTS = hash-test builtin-test coverage-test

lib_LTLIBRARIES = libhash.la
LDADD=libhash.la
libhash_la_SOURCES = hashmap.c

hash_test_SOURCES = hashtest.c

builtin_test_CPPFLAGS = -DHASHMAP_TEST=1
builtin_test_SOURCES = hashmap.c

hash_bench_CPPFLAGS = -DNDEBUG
hash_bench_CFLAGS = -O2
hash_bench_SOURCES = benchmark.c

bench: hash-bench
	./hash-bench

CLEANFILES = *.gcda *.gcno *.gcov *.o
