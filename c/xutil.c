#include "xutil.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>

FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = strcmp(path, "-") ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

void *
xrealloc(void *buf, size_t s, size_t count, void *iterator)
{
	ptrdiff_t offset;
	if( iterator != NULL ) {
		offset = *(char **)iterator - (char *)buf;
	}

	buf = realloc(buf, s * count);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}

	if( iterator != NULL ) {
		*(char **)iterator = (char *)buf + offset;
	}
	return buf;
}

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if(rv == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return rv;
}

int
die(const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

int
xopen(const char *path, int flags)
{
	int f = open(path, flags);
	if( f == -1 ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return f;
}

char *
xstrdup(const char *s1)
{
	char *r = strdup(s1);
	if( r == NULL ) {
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	return r;
}
