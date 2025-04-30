
/* Demonstrate that .1 * .1 gives produces a value that is *not*
 * the closest representable number to .01
 */

#include <stdio.h>
#include <math.h>

int
main(int argc, char **argv)
{
	double x = .1;
	double r = x * x;
	printf(".1 * .1 = %30.30f\n", r);
	for(int i = 0; i < 2; i += 1 ){
		r = nextafter(r, 0.0);
		printf("      --> %30.30f\n", r);
	}
	return 0;
}
