/* Grow an array of char */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include "xutil.h"

struct buffer {
	char *start;
	char *end;
	size_t cap;
};

void push(int c, struct buffer *);
int pop(struct buffer *);

int
main(int argc, char **argv)
{
	int c;
	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	struct buffer a = {0};

	while( (c = fgetc(ifp)) != EOF ) {
		push(c, &a);
	}
	while( (c = pop(&a)) != EOF ) {
		putchar(c);
	}
	return 0;
}

int
pop(struct buffer *a)
{
	return (a->start < a->end) ? *a->start++ : EOF;
}

void
push(int c, struct buffer *b)
{
	if( b->start == NULL || b->end >= b->start + b->cap ) {
		b->start = xrealloc(b->start, b->cap += BUFSIZ, 1, &b->end);
	}
	*b->end++ = c;
}
