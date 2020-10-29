#include <limits.h>
#include <stdio.h>
#include <stdint.h>

int
main(void)
{
	long x = 5;
	char buf[ sizeof(long) * CHAR_BIT ];
	printf("SIZE_MAX  = %zu\n", SIZE_MAX);
	printf("INT_MAX   = %d\n", INT_MAX);
	printf("UINT_MAX  = %u\n", UINT_MAX);
	printf("LONG_MAX  = %ld\n", LONG_MAX);
	printf("ULONG_MAX = %lu\n", ULONG_MAX);
	printf(" x == %ld is of size %zd\n", x, sizeof x);
	return 0;
}
