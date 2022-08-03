
#include <complex.h>
#include <stdio.h>

int
main(void)
{
	double complex z = 1.0 + 1.0 * I;  // z = 1 + i
	float complex a;
	double b = 1.0;

	a = (float complex) b; /* sets imaginary part to 0 */
	printf("a = %f + %fi\n", crealf(a), cimagf(a));
	printf("z = %lf + %lfi\n", creal(z), cimag(z));
	printf("|z| = %lf\n", cabs(z));

	double complex j = conj(z);
	printf("conj z = %lf + %lfi\n", creal(j), cimag(j));
	return 0;
}
