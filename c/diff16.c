
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Compare two files, 16 bytes at a time. (Purely to demonstrate memcmp.
 * Clearly, this should be implemented with getc.)
 */

FILE * xfopen(const char *, const char *);
size_t xfread(void *, FILE *, const char *);


int
main(int argc, char **argv)
{
	FILE *fp[2];
	size_t n[2];
	char buf[2][16];
	unsigned count = 0;

	if(argc != 3) { return EXIT_FAILURE; }
	fp[0] = xfopen(argv[1], "r");
	fp[1] = xfopen(argv[2], "r");
	do {
		n[0] = xfread(buf[0], fp[0], argv[1]);
		n[1] = xfread(buf[1], fp[1], argv[2]);
		if( n[0] != n[1] || (n[0] && memcmp(buf[0], buf[1], n[0]))) {
			fprintf(stderr, "files differ in block %u\n", count);
			return 1;
		}
		count += 1;
	} while(n[0]);
	puts("files are identical");
	return 0;
}

size_t
xfread(void *b, FILE *fp, const char *name)
{
	size_t n = fread(b, 1, 16, fp);
	if(n == 0 && ferror(fp)) {
		fprintf(stderr, "Error reading %s\n", name);
		exit(EXIT_FAILURE);
	}
	return n;
}

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
