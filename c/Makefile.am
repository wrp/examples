
bin_PROGRAMS = calc ctime fread getdelim mmap prctl qsort read scanf seek stat va_args
TESTS = check-calc calc-coverage
noinst_LTLIBRARIES = libringbuf.la
calc_LDADD = libringbuf.la
libringbuf_la_SOURCES = ring-buffer.c
CLEANFILES = *.gcda *.gcno
