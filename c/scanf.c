#include <stdio.h>
#include <stdarg.h>

void
get(const char *fmt, ...)
{
	va_list ap;
	int r;
	va_start(ap, fmt);
	r = vscanf(fmt, ap);
	va_end(ap);
	printf("fmt: %s, returned %d", fmt, r );
	if(r != EOF) {
		fputs(", scanned:\n", stdout);
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
	putchar('\n');
}


int
main(void)
{
	int v;
	char s[16];

	/* Read up to 15 chars up to the first whitespace.  Append terminating nul. */
	get("%15s", s);
	get("%d", &v);
}
