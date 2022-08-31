#include <stdio.h>

int
main(int argc, char **argv)
{

	/* For C 2017, __STDC_VERSION__ is 201710L. */
	printf("standard: %ld\n", __STDC_VERSION__);
	return 0;
}
