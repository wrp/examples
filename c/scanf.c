#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int
get(int expected, const char *fmt, ...)
{
	va_list ap;
	int r;
	va_start(ap, fmt);
	r = vscanf(fmt, ap);
	va_end(ap);
	printf("fmt: %s, returned %d", fmt, r );
	putchar('\n');
	if( r != expected ) {
		int c;
		c = getchar();
		printf( "expected %d: scan failed at '%c'\n", expected, c );
		exit(EXIT_FAILURE);
	}
	return r;
}


int
main(void)
{
	int a,b,c,v;
	char s[16];

	/* Read up to 15 chars up to the first whitespace.  Append terminating nul. */
	get(1, "%15s", s);
	printf("s = %s\n", s);
	get(1, "%d", &v);
	printf("v = %d\n", v);

	get(4, "%d%d%d%d", &a, &b, &c, &v);
	printf("a = %d, b = %d, c = %d, d = %d\n", a, b, c, v);
}
