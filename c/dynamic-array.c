/* Grow an array of char */

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
FILE * xfopen(const char *path, const char *mode);

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

void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	void **endp = endvp;
	ptrdiff_t offset = endp && *endp ? *endp - buf : 0;
	buf = realloc(buf, num * siz);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ) {
		*endp = buf + offset;
	}
	return buf;
}

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = path[0] != '-' || path[1] != '\0' ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ){
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}
