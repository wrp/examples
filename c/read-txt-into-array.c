/*
 * Read each line of a file into an array.
 */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

int append_line(FILE *fp, char ***data, size_t *siz, size_t idx);
void * xrealloc(void *buf, size_t num, size_t siz);
FILE * xfopen(const char *path, const char *mode);

int
main(int argc, char **argv)
{
	char **data = NULL;
	size_t siz = 0;
	size_t line_count = 0;
	FILE *fp = argc > 1 ? xfopen(argv[1],"r") : stdin;

	while( append_line(fp, &data, &siz, line_count)) {
		line_count += 1;
	}
	for( argv += argc > 2 ? 2 : argc; *argv; argv++ ) {
		int i = strtoul(*argv, NULL, 10);
		if( i > 0 && i <= (int)line_count ) {
			printf("line %d:\t%s", i, data[i - 1]);
		} else if( i < 0 && (int)line_count + i >= 0 ) {
			printf("line %d:\t%s", i, data[line_count + i]);
		}
	}
}

/* Read one line from fp into data */
int
append_line(FILE *fp, char ***data, size_t *siz, size_t idx)
{
	size_t cap;
	if( *siz <= idx ) {
		*data = xrealloc(*data, *siz += 128, sizeof *data);
	}
	(*data)[idx] = NULL;

	return getline((*data) + idx, &cap, fp) != -1;
}

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = path[0] != '-' || path[1] != '\0' ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

void *
xrealloc(void *buf, size_t num, size_t siz)
{
	buf = realloc(buf, num * siz);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return buf;
}
