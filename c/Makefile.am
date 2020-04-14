

noinst_HEADERS = xutil.h stack.h

noinst_PROGRAMS = ctime getdelim mmap prctl qsort read seek stat va_args
check_PROGRAMS = \
	random-word \
	calc \
	fread \
	scanf \
	access \
	arithperm \
	clean-man \
	dynamic \
	dynamic-read \
	fsync \
	grow \
	list \
	load-matrix \
	pipehup \
	poly \
	pyramid \
	read-indexed \
	scanf-term \
	tac \
	udp-client \
	udp-server

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
	tac-test \
	udp-test \
	coverage-test  # Must run last

noinst_LTLIBRARIES = libringbuf.la libstack.la libxutil.la
calc_LDADD = libringbuf.la libstack.la
LDADD=libxutil.la
libringbuf_la_SOURCES = ring-buffer.c
libstack_la_SOURCES = stack.c
libxutil_la_SOURCES = xutil.c
CLEANFILES = *.gcda *.gcno *.gcov
