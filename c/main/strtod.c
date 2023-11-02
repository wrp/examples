#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char **argv)
{
	char *end;
	double d;
	(void) argc;

	for( argv += 1; *argv; argv += 1 ) {
		d = strtod(*argv, &end);
		printf("%s parses as %g", *argv, d);
		if(*end) {
			printf(", ending at %c\n", *end);
		}
		putchar('\n');
	}
	return 0;
}
