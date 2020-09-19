#include <stdio.h>

/* Demonstrate that snprintf writes n-1 bytes of data plus a null byte.
 * That is, the first argument will be a properly terminated string.
 */

int
main(void)
{
	char v[5];
	int r;

	r = snprintf(v, sizeof v, "abcdefghijk" );
	printf("sizeof v == %zd\n", sizeof v);
	printf("snprintf(v, sizeof v, \"abcdefghijk\" ); returns %d\n", r);
	for(unsigned i = 0; i < sizeof v; i++ ) {
		printf("v[%d] = '%c' == %02x\n", i, v[i], v[i]);
	}

	return 0;
}
