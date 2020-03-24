
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args \
	access \
	grow \
	pyramid \
	dynamic-read \
	pipehup \
	list \
	read-indexed \
	scanf-term \
	fsync \
	udp-client \
	udp-server \
	dynamic \
	arithperm \
	poly \
	clean-man

TESTS = calc-test \
	fread-test \
	read-indexed-test \
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

noinst_LTLIBRARIES = libringbuf.la libstack.la libxutil.la
calc_LDADD = libringbuf.la libstack.la
LDADD=libxutil.la
libringbuf_la_SOURCES = ring-buffer.c
libstack_la_SOURCES = stack.c
libxutil_la_SOURCES = xutil.c
CLEANFILES = *.gcda *.gcno *.gcov
