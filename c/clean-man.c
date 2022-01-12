/*
 * Clean up man pages.
 * Discard octet before 0x08.
 * Discard all between 0x1b and 'm'
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "xutil.h"

int
main( int argc, char **argv )
{
	int c;
	char *defaults[] = { "-", NULL };

	if( argc > 1 && strcmp(argv[1], "-h") == 0 ){
		char *base = strrchr(argv[0], '/');
		printf("usage: %s [file ... ]", base ? base + 1 : argv[0]);
		puts("\n\nRemove all bytes preceding 0x08 and all between");
		puts("0x1b and the next 'm'.  Useful for naive clean up of");
		puts("manpages.");
		return 0;
	}
	char **args = argc > 1 ? ++argv : defaults;

	for( char **arg = args; *arg; arg++ ){
		FILE *ifp = xfopen(*arg, "r");

		int prev = 0x08;
		int state = 1;
		assert( EOF != 0x08 );
		do {
			c = getc(ifp);
			if( prev == 0x1b ) {
				state = 0;
			}
			if( state && c != 0x08 && prev != 0x08 ){
				putc(prev, stdout);
			}
			if( prev == 'm' ){
				state = 1;
			}
			prev = c;
		} while( c != EOF );
		fclose(ifp);
	}
}
