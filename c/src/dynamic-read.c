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
	char *prev;  /* start of data from previous read */
	char *end;  /* one past last char read from input */
};


static void
init_read_buf(struct read_buf *b)
{
	b->size = BUFSIZ;
	b->data = xrealloc(NULL, b->size + BUFSIZ, sizeof b->data, NULL);
	b->prev = b->end = b->s = b->data + BUFSIZ;
	/* We read into s, and leave space before s in order to copy
	** the unused values of the previous read. */
}

int
main(int argc, char **argv)
{
	ssize_t rc;

	struct read_buf b;

	init_read_buf(&b);

	int fd = argc > 1 ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;

	while(( rc = read(fd, b.s, BUFSIZ)) > 0 ){
		char *eol; /* A newline, or one past valid data */
		b.end = b.s + rc;

		if( (eol = findchr(b.s, b.end, '\n')) == b.end ) {
			/* No newlines found in the last read.  Read more. */
			if( b.end > b.data + b.size ){
				ptrdiff_t p_off = b.prev - b.data;
				b.size += BUFSIZ;
				b.data = xrealloc(b.data, BUFSIZ + b.size, sizeof *b.data, &b.end);
				eol = b.end;
				b.prev = b.data + p_off;
			}
			b.s = b.end;
			assert( b.s <= b.data + b.size );
			continue;
		}
		b.s = b.prev;
		do {
			assert( *eol == '\n' );
			assert( eol < b.end );
			reverse( b.s, eol - 1 );
			b.s = eol + 1;
			assert( b.s <= b.end );
		} while( (eol = findchr(b.s, b.end, '\n')) < b.end );
		assert( eol == b.end );
		assert( eol[-1] != '\n' || b.s == b.end );

		fwrite(b.prev, 1, b.s - b.prev, stdout);
		b.prev = b.data + BUFSIZ - (b.end - b.s);
		memcpy(b.prev, b.s, b.end - b.s);
		b.s = b.data + BUFSIZ;
	}
	if( rc == -1 ){
		perror(argc > 1 ? argv[1] : "stdin"); /* uncovered */
		return EXIT_FAILURE;                  /* uncovered */
	}
	if( b.prev < b.s ){
		reverse(b.prev, b.s - 1);
		fwrite(b.prev, 1, b.s - b.prev, stdout);
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
