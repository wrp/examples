/* Grow an array of char */

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);

struct buffer {
	char *start;
	char *end;
	size_t cap;
};

void push(int c, struct buffer *);
int pop(struct buffer *);

int
main(void)
{
	int c;
	struct buffer a = {0};

	while( (c = fgetc(stdin)) != EOF ){
		push(c, &a);
	}
	/* At this point, a.start contains all the data that was read */
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
