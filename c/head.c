#include "xutil.h"
#include <err.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char *end = "";
	int count = argc > 1 ? strtol(argv[1], &end, 10) : 10;
	char *const defaults[] = { "-", NULL };
	char *const*args = argc > 2 ? argv + 2: defaults;

	if( argv[1] && !strcmp(argv[1], "-h")) {
		printf("usage: %s [count] [path ...]\n", basename(argv[0]));
		exit(EXIT_SUCCESS);
	}
	if( *end ) {
		errx(EXIT_FAILURE, "Invalid char (%c) at position %td in %s",
			*end, end - argv[1] + 1, argv[1]);
	}
	for( ; *args; args++ ) {
		int c, line = 0;
		FILE *in = xfopen(*args,"r");
		if( argc > 3 ) {
			printf("******** %s ********\n", *args);
		}
		while( line < count && ( c = fgetc(in)) != EOF ) {
			line += putchar(c) == '\n';
		}
		if(fclose(in)) {
			err(EXIT_FAILURE, "close %s", *args);
		}
	}
}
