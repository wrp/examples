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
	struct buffer input;
	int c;

	grow(&input);
	while( (c = fgetc(fp)) != EOF ) {
		if(input.end == input.start + input.s) {
			grow(&input);
		}
		*input.end++ = c;
	}
	printf("%s", input.start);

}


void
grow(struct buffer *b)
{
	size_t siz;
	ptrdiff_t offset;

	if(b->start == NULL) {
		siz = 1024;
		offset = 0;
	} else {
		assert(b->end != NULL);
		siz = b->s * 2;
		offset = b->end - b->start;
	}
	b->start = realloc(b->start, siz);
	if (b->start == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
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
