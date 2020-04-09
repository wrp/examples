#include <stdio.h>
#include "xutil.h"

/*
 * Read a file in its entirety, then write the lines in reverse order.
 */

struct line { char *b; ssize_t s; size_t c; };

int
main(int argc, char *argv[])
{
	FILE *in = argc > 1 ? xfopen(argv[1], "r") : stdin;
	size_t siz = 2;
	struct line *lines = xmalloc(siz * sizeof *lines);
	struct line *t = lines;

	while( t->b = NULL, (t->s = getline(&t->b, &t->c, in)) != -1) {
		if( ++t == lines + siz ) {
			lines = xrealloc(lines, sizeof *lines, siz *= 2, &t);
		}
	}
	free(t->b);
	while( t-- > lines ) {
		fwrite(t->b, 1, t->s, stdout);
		free(t->b);
	}
	free(lines);
	fclose(in);
	return 0;
}
