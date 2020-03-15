/*
 * Reverse the lines of input.
 * Demonstrates using read and growing the buffer.
 *
 * Note that I continue to see segfaults when
 * running the test on a file that is mutating.
 */

#include <assert.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "xutil.h"

int xopen(const char *, int);
void reverse(char *, char *);
char * findchr(char *, char *, char);

int
main(int argc, char **argv)
{
	ssize_t rc;

	size_t siz = BUFSIZ;         /* size available to read into */
	char *buf = xrealloc(NULL, BUFSIZ + siz); /* Pad the front */
	char *s = buf + BUFSIZ;      /* first char of a line */
	char *prev = s;              /* start of data from previous read */
	char *end = s;               /* one past last char read from input */
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;

	while(( rc = read( fd, s, BUFSIZ )) > 0 ) {
		char *eol; /* A newline, or one past valid data */
		end = s + rc;

		if( (eol = findchr(s, end, '\n')) == end ) {
			/* No newlines found in the last read.  Read more. */
			if( end > buf + siz ) {
				ptrdiff_t e_off = end - buf;
				ptrdiff_t p_off = prev - buf;
				siz += BUFSIZ;
				buf = xrealloc(buf, BUFSIZ + siz);
				eol = end = buf + e_off;
				prev = buf + p_off;
			}
			s = end;
			assert( s <= buf + siz );
			continue;
		}
		s = prev;
		do {
			assert(*eol == '\n');
			assert(eol < end);
			reverse(s, eol-1);
			s = eol + 1;
			assert(s <= end);
		} while( (eol = findchr(s, end, '\n')) < end );
		assert(eol == end);
		assert(eol[-1] != '\n' || s == end);

		fwrite(prev, 1, s - prev, stdout);
		prev = buf + BUFSIZ - (end - s);
		memcpy(prev, s, end - s);
		s = buf + BUFSIZ;
	}
	if(rc == -1) {
		perror(argc > 1 ? argv[1] : "stdin"); /* uncovered */
		return EXIT_FAILURE;                  /* uncovered */
	}
	if(prev < s) {
		reverse(prev, s-1);
		fwrite(prev, 1, s - prev, stdout);
	}

	return EXIT_SUCCESS;
}

/*
 * Find v between str and end.  If not found,
 * return end.  (This is basically memchr, but
 * returns pointer to end if not found.)
 */
char *
findchr(char *str, char *end, char v) {
	assert(str <= end);
	while( str < end && *str != v )
		str += 1;
	return str;
}

void
reverse(char *start, char *end)
{
	for( ; start < end; start++, end-- ) {
		char tmp = *end;
		*end = *start;
		*start = tmp;
	}
}


int
xopen(const char *path, int flag)
{
	int fd = open(path, flag);
	if( fd == -1 ) { perror(path); exit(EXIT_FAILURE); }
	return fd;
}
