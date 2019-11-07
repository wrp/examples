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

/* Reverse a 4 character string */
void rev4(char *a) {
	assert(strlen(a) == 4);
	for(int i=0; i < 2; i++) {
		char t = a[i];
		a[i] = a[3-i];
		a[3-i] = t;
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
	rev4(v);
	b = strtol(v, NULL, 10);
	return b - a;
}

int
main(void)
{
	for( int i = 0; i < 10000; i++ ) {
		int v = i;
		int count = 0;
		while(v != 0 && v != 6174) {
			v = process(v);
			count += 1;
		}
		printf("%d: %d -> %d\n", i, count , v);
	}
	return 0;
}
