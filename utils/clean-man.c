/*
 * Clean up man pages.
 * Discard octet before 0x08.
 * Discard all between 0x1b and 'm'
 */
#include "config.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * xfopen(const char *path, const char *mode);
FILE * xtmpfile(char *tempname, size_t siz, const char *mode);
int die(const char *fmt, ...);
void xrename(const char *old, const char *new);


int
main(int argc, char **argv)
{
	int c;
	char *defaults[] = { "-", NULL };

	if( argc > 1 && strcmp(argv[1], "-h") == 0 ){
		char *base = strrchr(argv[0], '/');
		char *name = base ? base + 1 : argv[0];
		printf("usage: %s [file ... ]\n", name);
		puts("");
		printf("%s version: %s\n", name, PACKAGE_VERSION);
		puts("Remove all bytes preceding 0x08 and all between");
		puts("0x1b and the next 'm'.  Useful for naive clean up of");
		puts("manpages.  Also trim dos newlines.");
		return 0;
	}

	char **args = argc > 1 ? ++argv : defaults;

	for( char **arg = args; *arg; arg++ ){
		char tmpname[PATH_MAX];
		FILE *ifp = xfopen(*arg, "r");
		FILE *ofp = ifp == stdin ? stdout :
			xtmpfile(tmpname, sizeof tmpname, "w");

		int prev = 0x08;
		int state = 1;
		assert( EOF != 0x08 );
		do {
			c = getc(ifp);
			if( prev == 0x1b ){
				state = 0;
			}
			if( state && c != 0x08 && prev != 0x08 && c != '\r' ){
				putc(prev, ofp);
			}
			if( prev == 'm' ){
				state = 1;
			}
			prev = c;
		} while( c != EOF );
		if( ifp != stdin && fclose(ifp) ){
			die("%s: %s\n", *arg, strerror(errno));
		}
		if( ofp != stdout ){
			fclose(ofp);
			xrename(tmpname, *arg);
		}
	}
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

FILE *
xtmpfile(char *tempname, size_t siz, const char *mode)
{
	const char *template = "xutil.XXXXXXXXX";
	int fd;
	char *tmpdir = getenv("TMPDIR");
	int length = 0;

	if( tmpdir ){
		length = snprintf(tempname, siz - 1, "%s", tmpdir);
		if( length < siz - 1 && tempname[length - 1] != '/' ){
			tempname[length++] = '/';
		}
	}
	tempname[length] = '\0';

	strncat(tempname, template, siz - length);

	if( (fd = mkstemp(tempname)) == -1 ){
		perror(tempname);
		exit(EXIT_FAILURE);
	}
	FILE *fp = fdopen(fd, mode);
	if( fp == NULL ){
		perror(tempname);
		exit(EXIT_FAILURE);
	}
	return fp;
}

int
die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

void
xrename(const char *old, const char *new)
{
	if( rename(old, new) ){
		int errno_sav = errno;
		fprintf(stderr, "rename: %s -> ", old);
		perror(new);
		exit(EXIT_FAILURE);
	}
}
