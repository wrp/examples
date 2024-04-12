
/*
 * cat with an offset
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

FILE * xfopen(const char *path, const char *mode);
int
main(int argc, char **argv)
{
	FILE *fp = xfopen(argc > 1 ? argv[1] : "-", "r");
	off_t offset = argc > 2 ? strtoul(argv[2], NULL, 0) : 0;
	int c;
	if( offset ){
		if( argc > 1 ){
			if( fseek(fp, offset, SEEK_SET) ){
				perror(argv[1]);
				exit(EXIT_FAILURE);
			}
		}
		else {
			while( offset-- && fgetc(fp) != EOF ) {
				;
			}
		}
	}
	offset = argc > 3 ? strtoul(argv[3], NULL, 0) : 0;
	if( offset ) while( offset-- && (c = fgetc(fp)) != EOF ){
		putchar(c);
	} else while( (c = fgetc(fp)) != EOF ){
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
