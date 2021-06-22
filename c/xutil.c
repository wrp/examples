#include "xutil.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>

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

void *
xmalloc(size_t s)
{
	void *rv = malloc(s);
	if( rv == NULL ){
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
	if( f == -1 ){
		perror(path);
		exit(EXIT_FAILURE);
	}
	return f;
}

char *
xstrdup(const char *s1)
{
	char *r = strdup(s1);
	if( r == NULL ){
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	return r;
}

void *
xcalloc(size_t count, size_t size)
{
	void *r = calloc(count, size);
	if( r == NULL ){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	return r;
}
