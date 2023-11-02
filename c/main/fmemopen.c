#include <stdio.h>
#include <string.h>

/* Trivial demonstration of fmemopen */

int
main(int argc, char **argv)
{
	char *in = argc > 1 ? argv[1] : "hello, world!";
	char *repl = argc > 2 ? argv[2] : "cruel";
	char out[BUFSIZ];
	FILE *ip = fmemopen(in, strlen(in), "r");
	FILE *op = fmemopen(out, sizeof out, "w");
	int c;

	while( (c = fgetc(ip)) != ' ' && c && c != EOF ) {
		fputc(c, op);
	}
	fputc(' ', op);
	fputs(repl, op);
	fputc(' ', op);
	while( ( c = fgetc(ip)) != EOF && c ) {
		fputc(c, op);
	}

	fflush(op);
	printf("%s\n", out);
	return 0;
}
