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
	struct buffer b = {0};
	int c;

	while( (c = getchar()) != EOF && !isspace(c) ) {
		push(c, &a);
	}
	while(isspace(c = getchar())) {
		;
	}
	ungetc(c, stdin);
	while( (c = getchar()) != EOF ) {
		push(c, &b);
	}
	while( (c = pop(&a)) != EOF ) {
		putchar(c);
		if( (c = pop(&b)) != EOF ) {
			putchar(c);
		}
	}
	while( (c = pop(&b)) != EOF ) {
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
	if( b->start == NULL ) {
		b->end = b->start = xrealloc(NULL, b->cap = 8, 1, NULL);
	} else if( b->end >= b->start + b->cap ) {
		b->start = xrealloc(b->start, b->cap *= 2, 1, &b->end);
	}
	*b->end++ = c;
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *offsetp)
{
	ptrdiff_t offset;
	void **iterator = offsetp;
	if( iterator != NULL ) {
		offset = *iterator - buf;
	}
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
