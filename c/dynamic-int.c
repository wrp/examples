/* Read a stream of integers
 * Expected input is an opening [ followed by a comma separated
 * list of integers followed by a ].  For example: [ 3, 5, 6]
 * Input after the final `]` is ignored.
 */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>

struct int_buf {
	int *start;
	int *end;
	size_t cap;
};

void push(int c, struct int_buf *);
int pop(struct int_buf *a);
void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
FILE * xfopen(const char *path, const char *mode);

int
main(int argc, char **argv)
{
	int c;
	char k;
	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	struct int_buf a = {0};

	c = scanf(" %c ", &k); /* Read up to first [ */
	if( c != 1 || k != '[' ){
		fprintf(stderr, "invalid input\n");
		return EXIT_FAILURE;
	}
	while( 1 == fscanf(ifp, "%d , ", &c) ){
		push(c, &a);
	}
	c = scanf(" %c ", &k); /* Read up to final ] */
	if( c != 1 || k != ']' ){
		fprintf(stderr, "invalid input\n");
		return EXIT_FAILURE;
	}
	printf("[%d", pop(&a));
	while( (c = pop(&a)) != EOF ) {
		printf(", %d", c);
	}
	puts("]");
	return 0;
}

int
pop(struct int_buf *a)
{
	return a->start < a->end ? *a->start++ : EOF;
}

void
push(int c, struct int_buf *b)
{
	if( b->start == NULL || b->end >= b->start + b->cap ) {
		b->start = xrealloc(b->start, b->cap += BUFSIZ, 1, &b->end);
	}
	*b->end++ = c;
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	char **endp = endvp;
	char *b = buf;
	ptrdiff_t offset = b && endp && *endp ? *endp - b : 0;
	b = realloc(b, num * siz);
	if( b == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ){
		*endp = b + offset;
	}
	return b;
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

