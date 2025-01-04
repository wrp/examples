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
#pragma GCC diagnostic push  /* Save the current diagnostics */
#pragma GCC diagnostic ignored "-Woverride-init"
	int a[25] = {
		[0 ... 24] = -1,
		[3]       = 3,  /* Valid, warning suppressed (1)*/
	};
	int b[25] = {
		[0 ... 24] = -1,
		[3]       = 3,  /* Warning suppressed by pragma */
		[4]       = 4,
		[4 + 1]   = 5,
		[24] = 24,
#pragma GCC diagnostic pop  /* Restore previous diagnostics */
	};
	if( idx > 24 ) {
		fprintf(stderr, "Out of bounds\n");
	} else {
		printf("a[%1$d] = %2$d, b[%1$d] = %3$d\n", idx, a[idx], b[idx]);
	}
	return 0;
}

/*
 * (1)  To suppress the warning in gcc: -Wno-override-init
 *      To suppress in clang: -Wno-initializer-overrides
 */
