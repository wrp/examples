
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args \
	pyramid \
	pipehup \
	list \
	dynamic \
	clean-man
TESTS = calc-test \
	list-test \
	dynamic-test \
	pyramid-test \
	coverage-test
noinst_LTLIBRARIES = libringbuf.la
calc_LDADD = libringbuf.la
libringbuf_la_SOURCES = ring-buffer.c
CLEANFILES = *.gcda *.gcno *.gcov
