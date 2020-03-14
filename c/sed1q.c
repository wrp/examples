/*
 * Remove the first line from a file.  Inspired by:
 * https://stackoverflow.com/questions/60617223/remove-first-line-from-bigfile-using-bash
 */

#include <err.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

int is_regular(int, const char *);

int
main(int argc, char **argv)
{
	const char *rpath = argc > 1 ? argv[1] : "stdin";
	const char *wpath = argc > 1 ? argv[1] : "stdout";
	FILE *fr = argc > 1 ? xfopen(rpath, "r") : stdin;
	FILE *fw = argc > 1 ? xfopen(wpath, "r+") : stdout;
	char buf[BUFSIZ];
	int c;
	size_t rc;
	off_t length = 0;

	/* Discard the first line */
	while( (c = getc(fr)) != EOF && c != '\n' ) {
		;
	}
	if( c != EOF) while( (rc = fread(buf, 1, BUFSIZ, fr)) > 0) {
		size_t wc;
		wc = fwrite(buf, 1, rc, fw);
		length += wc;
		if( wc!= rc) {
			break;
		}
	}
	if( fclose(fr) ) {
		err(EXIT_FAILURE, "%s", rpath);
	}
	if( is_regular(fileno(fw), wpath) && ftruncate(fileno(fw), length)) {
		err(EXIT_FAILURE, "%s", wpath);
	}
	if( fclose(fw)) {
		err(EXIT_FAILURE, "%s", wpath);
	}
	return EXIT_SUCCESS;
}


int
is_regular(int fd, const char *name)
{
	struct stat s;
	if( fstat(fd, &s) == -1 ) {
		perror(name);
		exit(EXIT_FAILURE);
	}
	return !!(s.st_mode & S_IFREG);
}
