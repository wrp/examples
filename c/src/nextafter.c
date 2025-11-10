
/* Demonstrate that .1 * .1 gives produces a value that is *not*
 * the closest representable number to .01
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
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

	if(argc > 1) {
		float f = strtof(argv[1], NULL);
		double d = strtod(argv[1], NULL);
		long double ld = strtold(argv[1], NULL);

		float nf = nextafterf(f, INFINITY);
		double nd = nextafter(d, INFINITY);
		long double nld = nextafterl(ld, INFINITY);

		printf( "f     = %.119f\n", f);
		printf( "nextf = %.119f\n", nf );
		printf( "prevf = %.119f\n", nextafterf(f, -INFINITY));

		printf( "float : %g - %g = %g\n", nf, f, nf - f );
		printf( "double: %lg - %lg = %lg\n", nd, d, nd - d );
		printf( "long  : %Lg = %Lg = %Lg\n", nld, ld, nld - ld );
	}


	return 0;
}
