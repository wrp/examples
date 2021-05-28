/*
 * Read each line of a file into memory. Sort the lines and output.
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
compare_line(const void *va, const void *vb)
{
	char *a = *(char **)va;
	char *b = *(char **)vb;
	return strcmp(a, b);
}

int
main(int argc, char **argv)
{
	char **data = NULL;
	size_t siz = 0;
	size_t line_count = 0;
	FILE *fp = argc > 1 ? xfopen(argv[1], "r") : stdin;

	while( append_line(fp, &data, &siz, line_count) ){
		line_count += 1;
	}
	qsort(data, line_count, sizeof *data, compare_line);

	for( size_t i = 0; i < line_count; i += 1 ){
		printf("%8zd:\t%s", i + 1, data[i]);
	}
}

/* Read one line from fp into data[idx] */
int
append_line(FILE *fp, char ***data, size_t *siz, size_t idx)
{
	size_t cap = 0;
	if( *siz <= idx ){
		*data = xrealloc(*data, *siz += 128, sizeof **data);
	}
	(*data)[idx] = NULL;

	return getline((*data) + idx, &cap, fp) != -1;
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

void *
xrealloc(void *buf, size_t num, size_t siz)
{
	buf = realloc(buf, num * siz);
	if( buf == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return buf;
}
