#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* Take a 4 digit number.  Rearrange in ascending/descending order.
 * Find the difference.  Repeat
 *
 * This demonstrates the fixed points of Kaprekar's routine
 * https://en.wikipedia.org/wiki/6174_(number)
 */

int
compare_char( const void *a, const void *b )
{
	return *(char *)a > *(char *)b;
}

/* Reverse a string */
void
rev(char *a)
{
	for( char *e = a + strlen(a) - 1; a < e; a++, e-- ) {
		char t = *a;
		*a = *e;
		*e = t;
	}
}

int
process(int x)
{
	int a, b;
	if( x > 9999 ) {
		fprintf(stderr, "Invalid entry: %d\n", x);
		exit(EXIT_FAILURE);
	}
	char v[5];
	snprintf(v, 5, "%04d", x);
	assert(strlen(v) == 4);
	qsort(v, 4, 1, compare_char);
	a = strtol(v, NULL, 10);
	rev(v);
	b = strtol(v, NULL, 10);
	return b - a;
}

int
main(void)
{
	for( int i = 1; i < 10000; i++ ) {
		int v = i;
		while(v != 0 && v != 6174) {
			printf("%4d -> ", v);
			v = process(v);
		}
		printf("%d\n", v);
	}
	return 0;
}
