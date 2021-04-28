

noinst_HEADERS = xutil.h stack.h

noinst_PROGRAMS = ctime getdelim mmap prctl qsort read seek stat va_args head ncurses
check_PROGRAMS = \
	cat \
	random-word \
	calc \
	fread \
	scanf \
	access \
	arithperm \
	clean-man \
	dynamic \
	dynamic-read \
	dynamic-array \
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
	dynamic-array-test \
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

# Change this to lib_LTLIBRARIES to create installable libraries
noinst_LTLIBRARIES = libringbuf.la libstack.la libxutil.la
calc_LDADD = libringbuf.la libstack.la
ncurses_LDFLAGS = -lncursesw
ncurses_CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=600 -D_XOPEN_SOURCE_EXTENDED
LDADD=libxutil.la
libringbuf_la_SOURCES = ring-buffer.c
libstack_la_SOURCES = stack.c
libxutil_la_SOURCES = xutil.c
CLEANFILES = *.gcda *.gcno *.gcov
