#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char **argv)
{
	char *end;
	long n;
	int i;
	(void) argc;

	printf("longmax = %ld\n", LONG_MAX);
	printf("ulongmax = %lu\n", ULONG_MAX);
	for( argv += 1; *argv; argv += 1 ) {
		i = n = strtol( *argv, &end, 10);
		printf("%s parses as %ld and %d", *argv, n, i);
		if(*end) {
			printf(", ending at %c", *end);
		}
		putchar('\n');
	}
	return 0;
}
