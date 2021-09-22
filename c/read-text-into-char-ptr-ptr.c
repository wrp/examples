#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
FILE * xfopen(const char *path, const char *mode);
int die(const char *fmt, ... );

int
main(int argc, char **argv)
{
	char **map, **row, *end, *t;
	int c;

	FILE* file = argc > 1 ? xfopen(argv[1], "r") : stdin;
	map = xrealloc(NULL, 1, sizeof *map, &row);
	t = end = *row = NULL;

	while( (c = fgetc(file)) != EOF ){
		if( t == end ){
			*row = xrealloc(*row, end - *row + 64, 1, &t);
			end = t + 64;
		}
		*t++ = c;
		if( c == '\n' ){
			map = xrealloc(map, row - map + 2, sizeof *map, &row);
			t = end = *++row = NULL;
		}
	}
	for( char **r = map; r < row; r++ ){
		for( char *e = *r; *e != '\n'; e++ ){
			putchar(*e);
		}
		putchar('\n');
	}
	return 0;
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

int
die(const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
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

