#include <stdio.h>

/* demonstrate that snprintf write n - 1 bytes of data plus a null byte.
 * eg, the first argument will be a properly terminated string
 */

int
main(void)
{
	char v[5];

	snprintf( v, 5, "abcdefg" );
	for(unsigned i = 0; i < sizeof v; i++ ) {
		printf("v[%d] = '%c' == %02x\n", i, v[i], v[i]);
	}

	return 0;
}
