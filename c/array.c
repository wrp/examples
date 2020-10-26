#include <stdio.h>
#include <stdlib.h>

/* Demonstrate new syntax for initializing arrays
 * I believe the ... syntax is non-standard, but
 * is supported by clang and gcc
 */
int
main(int argc, char *argv[])
{
	unsigned idx = argc > 1 ? strtoul(argv[1], NULL, 0) : 0;
	int a[25] = {
		[0 ... 3] = 10,
		[3]       = 3,  /* Valid, but throws warning (1)*/
		[4]       = 4,
	};
	if( idx > 24 ) {
		fprintf(stderr, "Out of bounds\n");
	} else {
		printf("a[%d] = %d\n", idx, a[idx]);
	}
	return 0;
}

/*
 * (1)  To suppress the warning in gcc: -Wno-override-init
 *      To suppress in clang: -Wno-initializer-overrides
 */
