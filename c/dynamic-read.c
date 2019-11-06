/*
 * Reverse the lines of input.
 * Demonstrates using read and growing the buffer.
 */

#include <assert.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int xopen(const char *, int);
void * Realloc( void *buf, size_t s );
void reverse(char *, char *);
char * findchr(char *, char *, char);

int
main(int argc, char **argv)
{
	ssize_t rc;

	size_t siz = BUFSIZ;  /* size available to read into */
	char *buf = Realloc(NULL, BUFSIZ + siz); /* Pad the front */
	char *s = buf + BUFSIZ; /* first char of a line */
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;
	char *prev = s; /* start of unprocessed data from previous read */
	char *end = s; /* one past last char read from input */
	char *eol; /* A newline, or one past valid data */

	while(( rc = read( fd, s, BUFSIZ )) > 0 ) {
		end = s + rc;

		if( (eol = findchr(s, end, '\n')) == end ) {
			/* No newlines found in the last read.  Read more. */
			if( end < buf + siz ) {
				ptrdiff_t e_off = end - buf;
				ptrdiff_t p_off = prev - buf;
				siz += BUFSIZ;
				buf = Realloc(buf, BUFSIZ + siz);
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
		perror(argc > 1 ? argv[1] : "stdin");
		return EXIT_FAILURE;
	}
	if(prev < eol && eol[-1] != '\n') {
		reverse(prev, eol-1);
		fwrite(prev, 1, eol - prev, stdout);
	}
}

/*
 * Find v between str and end.  If not found,
 * return end.  (This is basically strchr, but
 * doesn't care about nul.)
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


void *
Realloc( void *buf, size_t s )
{
	buf = realloc( buf, s );
	if( buf == NULL) { perror("realloc"); exit(EXIT_FAILURE); }
	return buf;
}


int
xopen(const char *path, int flag)
{
	int fd = open(path, flag);
	if( fd == -1 ) { perror(path); exit(EXIT_FAILURE); }
	return fd;
}
