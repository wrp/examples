
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args \
	grow \
	pyramid \
	dynamic-read \
	pipehup \
	list \
	scanf-term \
	udp-client \
	udp-server \
	dynamic \
	arithperm \
	poly \
	clean-man
TESTS = calc-test \
	grow-test \
	arithperm-test \
	dynamic-read-test \
	list-test \
	udp-test \
	scanf-term-test \
	poly-test \
	dynamic-test \
	pyramid-test \
	coverage-test
noinst_LTLIBRARIES = libringbuf.la
calc_LDADD = libringbuf.la
libringbuf_la_SOURCES = ring-buffer.c
CLEANFILES = *.gcda *.gcno *.gcov
