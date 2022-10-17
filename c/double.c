

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * IEEE 754 representation of a double has 1 sign bit, 11 bits of exponent
 * and 52 bits for mantissa.  The 11 bit exponent is an unsigned value from
 * which 1023 is subtracted, but 0x000 and 0x7ff are special values, so range
 * of exponent is -1022 to +1023.  There is an implied 1 at the start of the
 * mantissa, so 1010 is treated as 1.1010 (eg 1 + .5 + .125)
 *
 * exponent 0x7ff represents inf (if mantissa == 0 ) and NaN (mant != 0)
 * exponent 0x000 represents +-0 (m == 0) and subnormals described by:
 * -1 ^ (sign) * 2 ^ -1022 * 0.fraction (the 52 bits)
 */


static void
show(unsigned long d)
{
	double xp = *(double *)&d;

	static_assert(sizeof d == sizeof xp, "Invalid sizes");

	char mantissa[55] = "1.";
	int exp = (d >> 52 & 0x7ffLU) - 1023LU;

	if( exp == -1023 ){
		mantissa[0] = '0';
		exp = -1022;
	}

	printf("%16lx", d);
	printf("\t%20g", xp);

	/* printf("\tsign = %c", d & (0x1LU << 63) ? '-' : '+'); */

	printf("\texp == %d", exp);

	for( int i = 0; i < 52; i += 1 ){
		mantissa[i + 2] = (d & (1LU << (51 - i))) ? '1' : '0';
	}

/*	printf("\tmantissa = %lx", d & ( (1LU << 52 ) - 1)); */
	printf("\tmantissa = %s", mantissa);
	putchar('\n');
}

static void show_str(const char *s);

int
main(int argc, char **argv)
{
	unsigned long d;

	if( argc < 2 ){
		d = 0x8000000000000000;
		for( int i = 0; i < 30; i += 1 ){
			show(d);
			d >>= 1;
		}

		show(0x7ff0000000000000);  /* inf */
		show(0x7ff000000ae00000);  /* NaN */
		show(0x3ff0000000000000);  /* 1 */
		show(0x3fe0000000000000);  /* 1 E -1 ( .5 ) */
		show(0x3ffa000000000000);  /* 1 + .5 + .125 */
		show(0x3fea000000000000);  /* ( 1 + .5 + .125 ) E -1 */
		show(0x4000000000000000);  /* 2 */
		show(0x4024000000000000);  /* 10   (1.0100E2) */
	}
	for( argv += 1; *argv; argv += 1 ){
		if( 0 == strcmp(*argv, "-" ) ){
			char buf[1024];
			while( 1 == scanf("%1023s", buf) ){
				show_str(buf);
			}
		} else {
			show_str(*argv);
		}
	}

	return 0;
}

static void
show_str(const char *s)
{
	char *end;
	printf("%20s ==> ", s);
	double x = strtod(s, &end);
	if(*end) {
		puts("invalid");
	} else {
		show(*(unsigned long *)&x);
	}
}
