/* Grow an array */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>

struct buffer {
	char *start;
	char *end;
	size_t cap;
};

void push(int c, struct buffer *);
int pop(struct buffer *);
void * xrealloc(void *buf, size_t num, size_t siz, void *offsetp);

int
main(void)
{
	struct buffer a = {0};
	int c;

	while( (c = getchar()) != EOF ) {
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
		b->start = xrealloc(b->start, b->cap += 128, 1, &b->end);
	}
	*b->end++ = c;
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *offsetp)
{
	void **iterator = offsetp;
	ptrdiff_t offset = iterator && *iterator ? *iterator - buf : 0;
	buf = realloc(buf, num * siz);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( iterator != NULL ) {
		*iterator = buf + offset;
	}
	return buf;
}
