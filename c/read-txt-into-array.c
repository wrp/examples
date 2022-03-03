/*
 * Read each line of a file into memory. Sort the lines and output.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct text_array {
	char **data;
	size_t len; /* length */
	size_t cap; /* capacity */
};

int append_line(FILE *fp, struct text_array *);
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
	struct text_array t = { 0 };
	FILE *fp = argc > 1 ? xfopen(argv[1], "r") : stdin;

	while( append_line(fp, &t) ){
		;
	}
	qsort(t.data, t.len, sizeof *t.data, compare_line);

	for( size_t i = 0; i < t.len; i += 1 ){
		printf("%8zd:\t%s", i + 1, t.data[i]);
	}
}

/* Read one line from fp into t */
int
append_line(FILE *fp, struct text_array *t)
{
	ssize_t status;
	size_t cap = 0;
	if( t->cap <= t->len ){
		t->data = xrealloc(t->data, t->cap += 128, sizeof *t->data);
	}
	t->data[t->len] = NULL;

	status = getline(t->data + t->len, &cap, fp);
	if( status != -1 ){
		t->len += 1;
	}
	return status != -1;
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
