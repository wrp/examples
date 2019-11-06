/*
 * Reverse the lines of input.
 * Demonstrates using read and growing the buffer.
 *
 * This utterly fails on long lines.  It's close,
 * but I've gotten bored.  Maybe come back to this some day.
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
	size_t rc;

	size_t siz = BUFSIZ;  /* size available to read into */
	char *buf = Realloc(NULL, BUFSIZ + siz); /* Pad the front */
	char *s = buf + BUFSIZ; /* position into which we read */
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;
	char *start = s;  /* start of unprocessed data in current read */
	char *prev = start; /* start of unprocessed data from previous read */

	while(( rc = read( fd, s, BUFSIZ )) > 0 ) {
		char *end = s + rc;
		int found = 0;
		char *eol;

		s = prev;
		while( (eol = findchr(s, end, '\n')) < end) {
			assert(*eol == '\n');
			reverse(s, eol-1);
			s = eol + 1;
			found = 1;
		}
		if( !found ) {
			/* No newlines found in the last read.  Read more. */
			if( buf + siz < end ) {
				ptrdiff_t s_off = s - buf;
				ptrdiff_t p_off = prev - buf;
				siz += BUFSIZ;
				buf = Realloc(buf, BUFSIZ + siz);
				start = buf + BUFSIZ;
				s = buf + s_off;
				prev = buf + p_off;
				end = s + rc;
			}
			s = end;
			continue;
		}
		memcpy(buf + BUFSIZ - (end - s), s, end - s);
		s = start;
		fwrite( prev, 1, end - prev, stdout );
	}
}

/*
 * Find v between str and end.  If not found,
 * return end.  (This is basically strchr, but
 * doesn't care about nul.)
 */
char *
findchr(char *str, char *end, char v) {
	assert(str <= end );
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
