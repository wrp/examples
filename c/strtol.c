#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
display(const char *arg, int base)
{
	char *end;
	long n;
	int i;
	i = n = strtol( arg, &end, base);
	printf("%s parses in base %d as %ld and %d", arg, base, n, i);
	if(*end) {
		printf(", ending at %c", *end);
	}
	putchar('\n');
}

int
main(int argc, char **argv)
{
	(void) argc;

	printf("longmax = %ld\n", LONG_MAX);
	printf("ulongmax = %lu\n", ULONG_MAX);
	for( argv += 1; *argv; argv += 1 ) {
		display(*argv, 8);
		display(*argv, 10);
		display(*argv, 16);
	}
	return 0;
}
