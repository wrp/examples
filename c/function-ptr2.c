
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef int(*func)(int, ...);

int bar(int a, ...)
{
	int b, c;
	va_list ap;
	va_start(ap, a);
	b = va_arg(ap, int);
	c = va_arg(ap, int);
	va_end(ap);
	return a + b - c;
}
int baz(int a, ...) {
	int b, c, d;
	va_list ap;
	va_start(ap, a);
	b = va_arg(ap, int);
	c = va_arg(ap, int);
	d = va_arg(ap, int);
	va_end(ap);
	return d*(a + b - c);
}
int
foo(func f, int t, ... ) {
	va_list ap;
	int a, b, c, d, rv = -1;
	char *fmt;
	va_start(ap, t);
	switch(t) {
	case 4:
		a = va_arg(ap, int);
	case 3:
		b = va_arg(ap, int);
		c = va_arg(ap, int);
		d = va_arg(ap, int);
	}
	switch(t) {
	case 4: rv = f(a,b,c,d); break;
	case 3: rv = f(b,c,d); break;
	case 2:
		fmt = va_arg(ap, char *);
		vfprintf(stderr, fmt, ap);
		break;
	}
	va_end(ap);
	return rv;
}

int
main(void)
{
	printf("%d\n", foo(bar, 3, 1, 2, 3));
	printf("%d\n", foo(baz, 4, 5, 3, 1, 2));
	printf("%d\n", foo(NULL, 2, "%s", "Hello, World!\n"));
	return 0;
}

