/* Read a file into memory */

#include <stdio.h>
#include <stdlib.h>

FILE * xfopen(const char *, const char *);
void * xrealloc(void *, size_t);

int
main(int argc, char **argv)
{
	size_t rc;
	size_t file_size = 0;
	size_t siz = BUFSIZ;
	char *buf = xrealloc(NULL, siz);
	FILE *fp = argc > 1 ? xfopen(argv[1],"r") : stdin;

	while( (rc = fread(buf + file_size, 1, BUFSIZ, fp)) == BUFSIZ ) {
		file_size += rc;
		siz += BUFSIZ;
		buf = xrealloc(buf, siz);
	}
	file_size += rc;
	fwrite(buf, 1, file_size, stdout);
}

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

void *
xrealloc(void *buf, size_t s)
{
	buf = realloc(buf, s);
	if( buf == NULL ) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return buf;
}
