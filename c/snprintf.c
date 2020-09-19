#include <stdio.h>

/* Demonstrate that snprintf writes n-1 bytes of data plus a null byte.
 * That is, the first argument will be a properly terminated string.
 */

int
main(void)
{
	char v[5];
	size_t len = 0;
	size_t siz = sizeof v;

	printf("sizeof v == %zd\n", sizeof v);

	len += snprintf(v, siz - len, "abcdefghijk" );

	printf("len += snprintf(v, siz - len, \"abcdefghijk\" );\n");
	printf("len = %zd\n", len);
	for(unsigned i = 0; i < sizeof v; i++ ) {
		printf("v[%d] = '%c' == %02x\n", i, v[i], v[i]);
	}
	int x = siz - len;

/*
	len += snprintf(v, siz - len, "abcdefghijk" );
	printf("len += snprintf(v, siz - len \"abcdefghijk\" );\n");
	*/
	printf("diff = %d, len = %zd\n", x, len);

	return 0;
}
