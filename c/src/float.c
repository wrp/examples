
/* ieee 754 double has 11-bit exponent, 52-bit significand, bias 1023
 * ieee 754 single has 8-bit exponent, 23-bit significand, bias 127
*/

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char fmt[256];  /* "%.120e" */

static void
init_fmt(void)
{
	int precision = 6;
	char format_char = 'e';
	char *p;
	char *f;
	if( (p = getenv("p")) != NULL ){
		char *end;
		precision = strtol(p, &end, 10);
		if( *end || precision < 1 || precision > 120 ){
			fprintf(stderr, "Invalid precision\n");
			exit(1);
		}
	}
	if( (f = getenv("f")) != NULL ){
		if( strchr("fFeEgGaA", *f) == NULL ){
			fprintf(stderr, "Invalid format character\n");
			exit(1);
		}
		format_char = f[0];
	}
	snprintf(fmt, sizeof fmt, "%%.%dl%c", precision, format_char);
}

static void
print_human(double v)
{
	int exp;
	v = frexp(v, &exp);
	printf("%7.3lfe%+d\t", v, exp);
}

enum width { none, dbl, flt };
void
show(const char *msg, double v, enum width context)
{
	union { double v; unsigned long k; } vu;
	vu.v = v;
	assert( sizeof vu.v == sizeof vu.k );

	if( context == dbl ){
		double prev = nextafter(v, -INFINITY);
		show("prev dbl", prev, 0);
	} else if( context == flt ){
		float prev = nextafterf(v, -INFINITY);
		show("prev sgl", prev, 0);
	}

	printf("%15s: 0x%016lx:\t", msg, vu.k);
	print_human(v);
	switch(fpclassify(v)){
	case FP_INFINITE:   printf("inf"); break;
	case FP_NAN:        printf("nan"); break;
	case FP_NORMAL:     printf("normal"); break;
	case FP_SUBNORMAL:  printf("sub"); break;
	case FP_ZERO:       printf("zero"); break;
	}
	putchar('\t');
	printf(fmt, v);
	putchar('\n');

	if( context == dbl ){
		show("next dbl", nextafter(v, INFINITY), 0);
	} else if( context == flt ){
		show("next sgl", nextafterf(v, INFINITY), 0);
	}
}

double
get_value(const char *s)
{
	char *end;
	union { double v; unsigned long k; } vu;
	if( *s == '0' && s[1] == 'x' ){
		vu.k = strtoul(s + 2, &end, 16);
	} else {
		vu.v = strtod(s, &end);
	}
	if( *end ){
		fprintf(stderr, "parse error at '%c' %s\n", *end, s);
		exit(1);
	}
	return vu.v;
}

int
main(int argc, char **argv)
{
	enum width context = dbl;
	if( argc > 1 && strcmp("float", argv[1]) == 0 ){
		context = flt;
		argv += 1;
		argc -= 1;
	}
	init_fmt();
	if( argc > 1 ){
		for( argv += 1; *argv; argv += 1 ){
			double d = get_value(*argv);
			char buf[23];
			snprintf(buf, sizeof buf, "%s", *argv);
			show(buf, d, context);
		}
		return 0;
	}

	show("Neg infinity", -INFINITY, context);
	show("Zero        ", 0.0, context);
	show("DBL_TRUE_MIN", DBL_TRUE_MIN, context);
	show("DBL_MIN", DBL_MIN, context);  /* Smallest normalized double */
	show("FLT_TRUE_MIN", FLT_TRUE_MIN, context);
	show("FLT_MIN", FLT_MIN, context);  /* Smallest normalized float */
	show("DBL_EPSILON", DBL_EPSILON, context);
	show("FLT_EPSILON", FLT_EPSILON, context);

	show("        One", 1.0, context);
	show("Middle float", 3e15, context);
	show("Middle double", 3e15, context);

	show("FLT_MAX", FLT_MAX, context);
	show("DBL_MAX", DBL_MAX, context);
	show("+infinity", INFINITY, context);

	return 0;
}


#if 0

See also:
     feclearexcept, fegetexceptflag, feraiseexcept, fesetexceptflag, fetestexcept – functions providing access to the floating-point status flags.
     fpclassify, isfinite, isinf, isnan, isnormal – classify a floating-point number
     isgreater, signbit



     FLT_MANT_DIG - The number of binary digits in the significand of a float.
     FLT_MIN_EXP - One more than the smallest exponent available in the float type.
     FLT_MAX_EXP - One more than the largest exponent available in the float type.
     FLT_DIG - the precision in decimal digits of a float.  A decimal value with this many digits, stored as a float, always yields the same value up to this many digits when converted back to decimal notation.
     FLT_MIN_10_EXP - the smallest n such that 10**n is a non-zero normal number as a float.
     FLT_MAX_10_EXP - the largest n such that 10**n is finite as a float.
     FLT_MIN - the smallest positive normal float.
     FLT_MAX - the largest finite float.
     FLT_EPSILON - the difference between 1.0 and the smallest float bigger than 1.0.

     On both OS X and iOS, the type double corresponds to IEEE-754 double precision.  A double-precision number is represented in 64 bits, and has a precision of 53 significant bits, roughly like 16 significant
     decimal digits.  11 bits are used to encode the exponent, which gives an exponent range from -1022 to 1023, inclusive.

     The header <float.h> defines several useful constants for the double type:
     DBL_MANT_DIG - The number of binary digits in the significand of a double.
     DBL_MIN_EXP - One more than the smallest exponent available in the double type.
     DBL_MAX_EXP - One more than the exponent available in the double type.
     DBL_DIG - the precision in decimal digits of a double.  A decimal value with this many digits, stored as a double, always yields the same value up to this many digits when converted back to decimal notation.
     DBL_MIN_10_EXP - the smallest n such that 10**n is a non-zero normal number as a double.
     DBL_MAX_10_EXP - the largest n such that 10**n is finite as a double.
     DBL_MIN - the smallest positive normal double.
     DBL_MAX - the largest finite double.
     DBL_EPSILON - the difference between 1.0 and the smallest double bigger than 1.0.

     On Intel macs, the type long double corresponds to IEEE-754 double extended precision.  A double extended number is represented in 80 bits, and has a precision of 64 significant bits, roughly like 19
     significant decimal digits.  15 bits are used to encode the exponent, which gives an exponent range from -16383 to 16384, inclusive.

     The header <float.h> defines several useful constants for the long double type:
     LDBL_MANT_DIG - The number of binary digits in the significand of a long double.
     LDBL_MIN_EXP - One more than the smallest exponent available in the long double type.
     LDBL_MAX_EXP - One more than the exponent available in the long double type.
     LDBL_DIG - the precision in decimal digits of a long double.  A decimal value with this many digits, stored as a long double, always yields the same value up to this many digits when converted back to
     decimal notation.
     LDBL_MIN_10_EXP - the smallest n such that 10**n is a non-zero normal number as a long double.
     LDBL_MAX_10_EXP - the largest n such that 10**n is finite as a long double.
     LDBL_MIN - the smallest positive normal long double.
     LDBL_MAX - the largest finite long double.
     LDBL_EPSILON - the difference between 1.0 and the smallest long double bigger than 1.0.

#endif
