#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char **argv)
{
	char *end;
	long n;
	(void) argc;

	for( argv += 1; *argv; argv += 1 ) {
		n = strtol( *argv, &end, 10);
		printf("%s parses as %ld", *argv, n);
		if(*end) {
			printf(", ending at %c\n", *end);
		}
		putchar('\n');
	}
	return 0;
}
