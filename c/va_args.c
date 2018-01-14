#include<stdio.h>
#include <stdarg.h>

void die(const char *fmt, ... )  __attribute__ ((format (printf, 1, 2)));
void die(const char *fmt, ... ) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

int main(void) {
	die("%d banana %s\n", 5, "foo");
}
