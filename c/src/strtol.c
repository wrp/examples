#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
display(const char *s, int base)
{
	char *end;
	long n;
	int i;
	unsigned long un;
	unsigned int ui;
	int errno_save;
	/*

     The strtol(), strtoll(), strtoimax(), and strtoq() functions
     return the result of the conversion, unless the value would
     underflow or overflow.  If no conversion could be performed,
     0 is returned and the global variable errno is set to EINVAL
     (the last feature is not portable across all platforms).  If
     an overflow or underflow occurs, errno is set to ERANGE and
     the function return value is clamped according to the following
     table.

           Function       underflow     overflow
           strtol()       LONG_MIN      LONG_MAX
           strtoll()      LLONG_MIN     LLONG_MAX
           strtoimax()    INTMAX_MIN    INTMAX_MAX
           strtoq()       LLONG_MIN     LLONG_MAX

	*/
	errno = 0;
	i = n = strtol(s, &end, base);
	errno_save = errno;
	printf("strtol : %s in base %d as (long)%ld and (int)%d", s, base, n, i);
	if( *end ){
		printf(", ending at %c", *end);
	}
	if( errno_save ){
		printf("; err: %s", strerror(errno_save));
	}
	putchar('\n');

	errno = 0;
	ui = un = strtoul(s, &end, base);
	errno_save = errno;
	printf("strtoul: %s in base %d as (ulong)%lu and (uint)%u", s, base,
		un, ui);
	if( *end ){
		printf(", ending at %c", *end);
	}
	if( errno_save ){
		printf("; err: %s", strerror(errno_save));
	}
	putchar('\n');
}

int
main(int argc, char **argv)
{
	(void) argc;

	printf("longmax = %ld\n", LONG_MAX);
	printf("ulongmax = %lu\n", ULONG_MAX);
	for( argv += 1; *argv; argv += 1 ){
		display(*argv, 0);
		display(*argv, 8);
		display(*argv, 10);
		display(*argv, 16);
	}
	return 0;
}
