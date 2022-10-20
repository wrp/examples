

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int rv = EXIT_SUCCESS;
	const char *path = argc > 1 ? argv[1] : "stdin";
	const char *mode = argc > 2 ? argv[2] : "r";
	FILE *fp = argc > 1 ? fopen(path, mode) : stdin;

	if( fp == NULL ){
		perror(path);
		rv = EXIT_FAILURE;
	}
	return rv;
}
