#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE * xfopen(const char *path, const char *mode);

int
next_file(FILE **input, int count, int idx)
{
	int next = (idx + 1) % count;
	while( next != idx && input[next] == NULL ){
		next = (next + 1) % count;
	}
	return input[next] ? next : -1;
}

int
main(int argc, char *argv[])
{
	if( argc < 2 ){
		char *base = strrchr(argv[0], '/');
		printf("usage: %s [file ...]\n", base ? base + 1 : argv[0]);
		printf("\nInterleave the lines of each file\n");
		return EXIT_SUCCESS;
	}
	FILE *input[argc - 1];
	for( int i = 0; i < argc - 1; i++ ){
		input[i] = xfopen(argv[i + 1], "r");
	}
	int c;
	int idx = 0;
	while( idx != -1 && input[idx] ){
		while( (c = getc(input[idx])) != EOF ){
			if( c == '\n' ) {
				idx = next_file(input, argc - 1, idx);
			}
			putchar(c);
		}
		if( ferror(input[idx]) || fclose(input[idx]) ){
			perror(argv[idx + 1]);
			return EXIT_FAILURE;
		}
		input[idx] = NULL;
		idx = next_file(input, argc - 1, idx);
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
