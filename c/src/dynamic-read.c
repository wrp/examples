/*
** Reverse each line of input.
** Demonstrate using read and growing the buffer.
**/

#include <assert.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "xutil.h"

void reverse(char *, char *);
char * findchr(char *, char *, char);

struct read_buf {
	char *data;
	size_t size;
	char *s;
};


static void
init_read_buf(struct read_buf *b)
{
	b->size = BUFSIZ;
	b->data = xrealloc(NULL, b->size + BUFSIZ, sizeof b->data, NULL);
}

int
main(int argc, char **argv)
{
	ssize_t rc;

	struct read_buf b;

	init_read_buf(&b);

	char *s = b.data + BUFSIZ;      /* first char of a line */
	char *prev = s;              /* start of data from previous read */
	char *end = s;               /* one past last char read from input */
	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;

	while(( rc = read(fd, s, BUFSIZ)) > 0 ){
		char *eol; /* A newline, or one past valid data */
		end = s + rc;

		if( (eol = findchr(s, end, '\n')) == end ) {
			/* No newlines found in the last read.  Read more. */
			if( end > b.data + b.size ){
				ptrdiff_t p_off = prev - b.data;
				b.size += BUFSIZ;
				b.data = xrealloc(b.data, BUFSIZ + b.size, sizeof *b.data, &end);
				eol = end;
				prev = b.data + p_off;
			}
			s = end;
			assert( s <= b.data + b.size );
			continue;
		}
		s = prev;
		do {
			assert( *eol == '\n' );
			assert( eol < end );
			reverse( s, eol - 1 );
			s = eol + 1;
			assert( s <= end );
		} while( (eol = findchr(s, end, '\n')) < end );
		assert( eol == end );
		assert( eol[-1] != '\n' || s == end );

		fwrite(prev, 1, s - prev, stdout);
		prev = b.data + BUFSIZ - (end - s);
		memcpy(prev, s, end - s);
		s = b.data + BUFSIZ;
	}
	if( rc == -1 ){
		perror(argc > 1 ? argv[1] : "stdin"); /* uncovered */
		return EXIT_FAILURE;                  /* uncovered */
	}
	if( prev < s ){
		reverse(prev, s - 1);
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
findchr(char *str, char *end, char v)
{
	assert( str <= end );
	while( str < end && *str != v ){
		str += 1;
	}
	return str;
}

void
reverse(char *start, char *end)
{
	for( ; start < end; start++, end-- ){
		char tmp = *end;
		*end = *start;
		*start = tmp;
	}
}
