
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args \
	access \
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
	fread-test \
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

noinst_LTLIBRARIES = libringbuf.la libstack.la libutil.la
LDADD = libringbuf.la libstack.la libutil.la
libringbuf_la_SOURCES = ring-buffer.c
libstack_la_SOURCES = stack.c
libutil_la_SOURCES = util.c
CLEANFILES = *.gcda *.gcno *.gcov
