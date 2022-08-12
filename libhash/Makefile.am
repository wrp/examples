
include_HEADERS = hashmap.h
check_PROGRAMS = hash-test builtin-test
TESTS = hash-test builtin-test coverage-test

lib_LTLIBRARIES = libhash.la
LDADD=libhash.la
libhash_la_SOURCES = hashmap.c

hash_test_SOURCES = hashtest.c

# This is absurd.  To actually get bench marks that matter at all,
# we need -DNDEBUG.  But all the testing in builtin-tests is done
# via assertions, so the test would be a no-op if we define NDEBUG.
# So we just accept that these bench marks are faulty and assertions
# remain enabled
builtin_test_CPPFLAGS = -DHASHMAP_TEST
builtin_test_SOURCES = hashmap.c

bench: builtin-test
	BENCH=1 ./builtin-test

CLEANFILES = *.gcda *.gcno *.gcov *.o
