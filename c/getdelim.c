#include <stdio.h>
#include <stdlib.h>

/* demonstrate getdelim to read an entire file */
FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = fopen(path, mode);
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

int
main(int argc, char **argv)
{
	char *input_path;
	char *output_path;
	ssize_t rc;
	char *this;
	size_t n = 0;
	char *input = NULL;
	FILE *ifp = argc > 1 ? xfopen(input_path = argv[1], "r") : stdin;
	FILE *ofp = argc > 2 ? xfopen(output_path = argv[2], "w") : stdout;

	if( (rc = getdelim( &input, &n, EOF, ifp)) == -1) {
		perror(input_path);
		return EXIT_FAILURE;
	}
	this = input + rc - 1;
	do {
		if(fputc((int)*this, ofp) == EOF) {
			perror(output_path);
			return EXIT_FAILURE;
		}
		this -=1;
	} while( this >= input);
	return EXIT_SUCCESS;
}
