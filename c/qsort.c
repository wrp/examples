#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* sort the arguments */

int
compare( const void *a, const void *b )
{
	return strcmp(*(char **)a,*(char **)b);
}

int
main( int argc, const char **argv )
{
	qsort( argv + 1, argc - 1, sizeof *argv, compare );
	for( int i = 1; i < argc; i++ ) {
		puts( argv[i] );
	}
	return 0;
}
