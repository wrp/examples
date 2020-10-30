/*
 * Simple implementation of cat, demonstrating how to read
 * a file completely into memory.
 */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "xutil.h"

struct buf {
	char *data;
	char *end;
	size_t capacity;
};

void grow(struct buf *);
int append(struct buf *, FILE *, size_t);


int
main(int argc, char **argv)
{
	struct buf b = { 0 };
	FILE *fp = argc > 1 ? xfopen(argv[1], "r") : stdin;

	while( append(&b, fp, BUFSIZ) ) {
		;
	}
	fwrite(b.data, 1, b.end - b.data, stdout);
}

/* Read n bytes from fp into b */
int
append(struct buf *b, FILE *fp, size_t n)
{
	size_t rc;
	assert( b->end <= b->data + b->capacity );
	while( b->data + b->capacity < b->end + n ) {
		grow(b);
	}
	rc = fread(b->end, 1, n, fp);
	b->end += rc;
	assert( b->end <= b->data + b->capacity );
	return rc == n;
}

void
grow(struct buf *b)
{
	b->capacity = b->capacity ? b->capacity * 2 : BUFSIZ;
	b->data = xrealloc(b->data, b->capacity, sizeof *b->data, &b->end);
}
