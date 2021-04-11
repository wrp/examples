/* Grow an array */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "xutil.h"

struct buffer {
	unsigned char *data;
	unsigned char *start;
	unsigned char *end;
	size_t cap;
};

void push(int c, struct buffer *);
int next(struct buffer *);

int
main(int argc, char **argv)
{
	int c;
	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	struct buffer a = {0};

	while( (c = fgetc(ifp)) != EOF ) {
		push(c, &a);
	}
	while( (c = next(&a)) != EOF ) {
		putchar(c);
	}
	return 0;
}

int
next(struct buffer *a)
{
	return (a->start < a->end) ? *a->start++ : EOF;
}

void
push(int c, struct buffer *b)
{
	if( b->data == NULL || b->end >= b->data + b->cap ) {
		ptrdiff_t offset[2] = {0, 0};
		if( b->data != NULL ){
			offset[0] = b->end - b->data;
			offset[1] = b->start - b->data;
		}
		b->data = realloc(b->data, b->cap += 128);
		if( b->data == NULL ){
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		b->end = b->data + offset[0];
		b->start = b->data + offset[1];
	}
	*b->end++ = c;
}
