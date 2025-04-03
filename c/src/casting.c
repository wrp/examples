#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	size_t s = INT_MAX + (unsigned)(INT_MAX - 5);
	int i = (int)s;
	long ll = argc > 1 ? strtol(argv[1], NULL, 10) : LONG_MAX;

	printf("size_t = %zd\n", s);
	printf("int = %d\n", i);

	i = 10;

	if( i < (int)s ) {
		printf("bogus comparison\n");
	}
	if( (unsigned) i < s ) {
		printf("ok comparison\n");
	}

	i = -5;
	printf("negative number cast as unsigned: %u\n", (unsigned)i);

	printf("long = %ld\n", ll);
	printf("(int)long = %d\n", (int)ll);
	return 0;
}
