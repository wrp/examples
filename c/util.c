#include "util.h"
#include <stdlib.h>
#include <string.h>

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = strcmp(path, "-") ? fopen(path, mode) : stdin;
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

void *
xrealloc(void *buf, size_t s)
{
	buf = realloc(buf, s);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return buf;
}
