#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

void die(const char *fmt, ... )  __attribute__ ((format (printf, 1, 2)));
void
die(const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}


int
foo(const char *fmt, ...)
{
	int sum = 0;
	int c, d;
	short s;
	long l;

	va_list ap;
	va_start(ap, fmt);
	while(*fmt) switch(*fmt++) {
	case 'c': c = va_arg(ap, int); printf("%c,", c); sum += c; break;
	case 'd': d = va_arg(ap, int); printf("%d,", d); sum += d; break;
	case 's': s = va_arg(ap, int); printf("%hd,", s); sum += s; break;
	case 'l': l = va_arg(ap, long); printf("%ld,", l); sum += l; break;
	}
	va_end(ap);
	putchar('\n');
	return sum;
}


int
main(void)
{
	long k = (long)INT_MAX + 35;
	foo("dcsl", 5, 'k', 2, k);
	foo("ddss", 5, 'k', 2, k);
	die("%d banana %s\n", 5, "foo");
	return 0;
}
