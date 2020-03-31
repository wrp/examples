
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args \
	access \
	grow \
	pyramid \
	dynamic-read \
	pipehup \
	list \
	read-indexed \
	load-matrix \
	scanf-term \
	fsync \
	udp-client \
	udp-server \
	dynamic \
	arithperm \
	poly \
	clean-man

TESTS = \
	arithperm-test \
	calc-test \
	dynamic-read-test \
	dynamic-test \
	fread-test \
	grow-test \
	list-test \
	mmap-test \
	poly-test \
	pyramid-test \
	read-indexed-test \
	scanf \
	scanf-term-test \
	scanf-test \
	udp-test \
	coverage-test  # Must run last

noinst_LTLIBRARIES = libringbuf.la libstack.la libxutil.la
calc_LDADD = libringbuf.la libstack.la
LDADD=libxutil.la
libringbuf_la_SOURCES = ring-buffer.c
libstack_la_SOURCES = stack.c
libxutil_la_SOURCES = xutil.c
CLEANFILES = *.gcda *.gcno *.gcov
