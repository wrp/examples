#include <stdio.h>
#include <string.h>
#include "xutil.h"

/* A simple implementation of tac.
 * Reads a file in its entirety, then writes
 * it in reverse order, linewise
 */

int
main(int argc, char *argv[])
{
	FILE *in = argc > 1 ? xfopen(argv[1], "r") : stdin;
	char **lines = NULL;
	size_t siz = 0;
	size_t s = 0;
	size_t line_number = 0;
	char *line = NULL;


	while( getline(&line, &s, in) != -1) {
		if( line_number + 1 > siz ) {
			lines = xrealloc(lines, sizeof *lines * (siz = siz ? siz * 2 : 128));
		}
		lines[line_number++] = xstrdup( line );
	}
	while( line_number > 0 ) {
		printf("%s",  lines[--line_number]);
	}
	return 0;
}
