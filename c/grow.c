/* Simple program to demonstrate growing a string while reading data. */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

struct buffer {
	char *start;
	char *end;
	size_t s;
};

FILE *xfopen(const char *, const char *);
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
	size_t siz;
	ptrdiff_t offset;

	if(b->start == NULL) {
		siz = 8;
		offset = 0;
	} else {
		assert(b->end != NULL);
		siz = b->s * 2;
		offset = b->end - b->start;
	}
	b->start = realloc(b->start, siz);
	if (b->start == NULL) { /* uncovered block */
		perror("realloc");
		exit(EXIT_FAILURE);
	} /* end uncovered */
	b->end = b->start + offset;
	b->s = siz;
}

FILE *
xfopen(const char *path, const char * mode)
{
	FILE *ret = fopen(path, mode);
	if(ret == NULL) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return ret;
}
