#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Process \b, \r, and \n.  Discard non-printable */

FILE * xfopen(const char *path, const char *mode);
off_t xftello(FILE *stream, const char *name);
void xfseeko(FILE *stream, off_t offset, int whence, const char *name);

int
main(int argc, char **argv)
{
	const char *mode = "w";
	char *name = strchr(argv[0], '/');
	off_t last = 0, max = 0, curr = 0;
	name = name ? name + 1 : argv[0];
	if( argc > 1 && ! strcmp(argv[1], "-a")) {
		argv += 1;
		argc -= 1;
		mode = "a";
	}
	if( argc > 1 && ! strcmp(argv[1], "-h")) {
		printf("usage: %s [-a] file [ file ...]\n", name);
		return EXIT_SUCCESS;
	}
	if( argc < 2 ) {
		fprintf(stderr, "Missing output file.  -h for usage\n");
		return EXIT_FAILURE;
	}
	assert( argc > 1 );
	argc -= 1;
	argv += 1;

	FILE *ofp[argc];
	for( int i = 0; i < argc; i++ ) {
		ofp[i] = xfopen(argv[i], mode);
	}
	int c;
	while( ( c = fgetc(stdin) ) != EOF ) {
		fputc(c, stdout);
		for( int i = 0; i < argc; i++ ) {
			if( c == '\b' ) {
				xfseeko(ofp[i], -1, SEEK_CUR, argv[i]);
			} else if( isprint(c) ) {
				fputc(c, ofp[i]);
			} else if( c == '\n' ) {
				xfseeko(ofp[i], max, SEEK_SET, argv[i]);
				fputc(c, ofp[i]);
				last = curr + 1;
			} else if( c == '\r' ) {
				xfseeko(ofp[i], last, SEEK_SET, argv[i]);
			}
		}
		curr = xftello(ofp[0], argv[0]);
		if( curr > max ) {
			max = curr;
		}
	}
	return 0;
}

off_t
xftello(FILE *stream, const char *name)
{
	off_t r = ftello(stream);
	if( r == -1 ) {
		perror(name);
		exit(EXIT_FAILURE);
	}
	return r;
}

void
xfseeko(FILE *stream, off_t offset, int whence, const char *name)
{
	if( fseeko(stream, offset, whence) ) {
		perror(name);
		exit(EXIT_FAILURE);
	}
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
