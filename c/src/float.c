

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	printf("Smallest float: %g\n", FLT_MIN);
	printf("Largest float: %g\n", FLT_MAX);
	return 0;
}


#if 0

See also:
     feclearexcept, fegetexceptflag, feraiseexcept, fesetexceptflag, fetestexcept – functions providing access to the floating-point status flags.

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
