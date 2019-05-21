/* Read a file into memory */

#include <stdio.h>
#include <stdlib.h>

FILE * Fopen(const char *path, const char *mode) {
	FILE *fp = fopen(path, mode);
	if( fp == NULL ) { perror(path); exit(EXIT_FAILURE); }
	return fp;
}

void * Realloc( void *buf, size_t s )
{
	buf = realloc( buf, s );
	if( buf == NULL) { perror("realloc"); exit(EXIT_FAILURE); }
	return buf;
}

int
main(int argc, char **argv)
{
	size_t rc;
	size_t file_size;

	size_t siz = BUFSIZ;
	char *buf = Realloc(NULL,siz);
	FILE *fp = argc > 1 ? Fopen(argv[1],"r") : stdin;

	while(( rc = fread( buf + file_size, 1, BUFSIZ, fp )) == BUFSIZ ) {
		file_size += rc;
		siz += BUFSIZ;
		buf = Realloc( buf, siz );
	}
	file_size += rc;
	fwrite( buf, 1, file_size, stdout );
}
