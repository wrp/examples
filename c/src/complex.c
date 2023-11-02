
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static void
show(double complex z)
{
	double imag = cimag(z);
	printf("%lg %c I * %lg", creal(z), imag < 0 ? '-' : '+', fabs(imag));
}

static void
display(double complex z, double complex c, double d, const char *msg, int t)
{
	printf("%10s ", msg);
	show(z);
	fputs(" = ", stdout);
	if(t)
		show(c);
	else
		printf("%lg", d);
	putchar('\n');
}

int
main(int argc, char **argv)
{
	double complex z = 1.0 + 1.0 * I;
	if( argc > 1 ){
		double r = strtod(argv[1], NULL);
		z = CMPLX(r, cimag(z));
	}
	if( argc > 2 ){
		double r = strtod(argv[1], NULL);
		z = CMPLX(creal(z), r);
	}

#define DOD(X) display(z, 0, X(z), #X, 0)
	DOD(creal);
	DOD(cimag);
	DOD(cabs);
	DOD(carg);

#define DOC(X) display(z, X(z), 0, #X, 1)
	DOC(cexp);
	DOC(clog);
	DOC(conj);
	DOC(cproj);
	DOC(csqrt);
	DOC(casinh);
	DOC(cacosh);
	DOC(catanh);
	DOC(csinh);
	DOC(ccosh);
	DOC(ctanh);
	DOC(casin);
	DOC(cacos);
	DOC(catan);
	DOC(csin);
	DOC(ccos);
	DOC(ctan);


	return 0;
}
