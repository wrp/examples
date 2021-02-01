#include <stdio.h>
#include <stdlib.h>

FILE * xfopen(const char *path, const char *mode);

int
main(int argc, char *argv[])
{
	FILE *input[2];
	int idx = 0;
	input[0] = xfopen(argc > 1 ? argv[1] : "input1", "r");
	input[1] = xfopen(argc > 2 ? argv[2] : "input2", "r");
	int c;
	while( (c = getc(input[idx])) != EOF ){
		if( c == '\n' ) {
			idx = !idx;
		}
		putchar(c);
	}
	while( (c = getc(input[!idx])) != EOF ){
		putchar(c);
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
