#include <stdio.h>
#include <string.h>
#include "xutil.h"

/*
 * Read a file in its entirety, then write the lines in reverse order.
 */

struct line { char *b; ssize_t s; size_t c; };

int
main(int argc, char *argv[])
{
	FILE *in = argc > 1 ? xfopen(argv[1], "r") : stdin;
	size_t siz = 128;
	struct line *lines = xmalloc(siz * sizeof *lines);
	struct line *t = lines;

	while( t->b = NULL, (t->s = getline(&t->b, &t->c, in)) != -1) {
		if( ++t > lines + siz ) {
			siz *= 2;
			lines = xrealloc(lines, sizeof *lines * siz);
		}
	}
	while( t-- > lines ) {
		fwrite(t->b, 1, t->s, stdout);
		/* fputs(t->b, stdout);*/
		free(t->b);
	}
	free(lines);
	return 0;
}
