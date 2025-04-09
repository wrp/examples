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
		printf("'%s' parses as %g\n", *argv, d);
		if(*end) {
			putchar(' ');
			for(char *a = *argv; a < end; a += 1) {
				putchar(' ');
			}
			printf("^-- ending at '%c'\n", *end);
		}
	}
	return 0;
}
