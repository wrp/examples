#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Check if the input is a palindrome. */
static FILE * xfopen(const char *path, const char *mode);

static int
matches(const char *s, const char *e)
{
	if( s > e ) {
		return 1;
	}
	while( ! isalpha(*s) && s < e ) {
		s += 1;
	}
	while( ! isalpha(*e) && e > s ) {
		e -= 1;
	}
	if( s == e ) {
		return 1;
	} else if( tolower(*s) == tolower(*e) ){
		return matches(s + 1, e - 1);
	} else {
		return 0;
	}
}

static int
is_palindrome(const char *p)
{
	return matches(p, p + strlen(p) - 1);
}

int
main(int argc, char **argv)
{
	int rv = 0;
	char *input_path;
	ssize_t rc;
	size_t n = 0;
	char *input = NULL;
	FILE *ifp = argc > 1 ? xfopen(input_path = argv[1], "r") : stdin;

	if( (rc = getdelim( &input, &n, EOF, ifp)) == -1 ) {
		perror(input_path);
		return EXIT_FAILURE;
	}
	if( ! is_palindrome(input) ) {
		fprintf(stderr, "Input is not a palindrome\n");
		rv = EXIT_FAILURE;
	}
	return rv;
}

static FILE *
xfopen(const char *path, const char *mode)
{
	FILE *fp = path[0] != '-' || path[1] != '\0' ? fopen(path, mode) :
		*mode == 'r' ? stdin : stdout;
	if( fp == NULL ) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

