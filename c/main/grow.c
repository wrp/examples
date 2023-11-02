/* Simple program to demonstrate growing a string while reading data. */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "xutil.h"

struct buffer {
	char *start;
	char *end;
	size_t s;
};

void grow(struct buffer *);

int
main(int argc, char **argv)
{
	FILE *fp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	size_t count;
	struct buffer input = {0};
	int c, status = EXIT_SUCCESS;

	grow(&input);
	while( (c = fgetc(fp)) != EOF ) {
		if(input.end == input.start + input.s) {
			grow(&input);
		}
		*input.end++ = c;
	}
	count = input.end - input.start;
	if( fwrite(input.start, 1, count, stdout) != count) { /* uncovered block */
		perror("stdout");
		status = EXIT_FAILURE;
	} /* end uncovered */
	return status;
}


void
grow(struct buffer *b)
{
	if(b->start == NULL) {
		b->s = 8;
		b->end = b->start;
	} else {
		assert(b->end != NULL);
		b->s *= 2;
	}
	b->start = xrealloc(b->start, b->s, sizeof *b->start, &b->end);
}
