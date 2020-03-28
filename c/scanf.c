#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

void show_buf(char *a, int);
int isstring(const char *s);
int scan(const char *input, const char *fmt, ...);


void
simple_examples(void)
{
	char buf[128] = "XXXXXXXXXXXXx";
	int k;
	scan("input string", "%3s", buf); /* Writes 4 chars: 'inp\0' */
	scan("input string", "%7s", buf); /* Writes 6 chars; 'input\0' */
	scan("input string\nline 2", "%[^\n]s", buf); /* Writes first line */
	scan("input string\nline 2", "%7[^\n]s", buf); /* Write 8 chars */
	scan("24", "%d", &k);
}

int
main(int argc, char **argv)
{
	int c;
	long ld;
	char a[7][1024];

	simple_examples();

	(void)argc;
	for(argv += 1; *argv; argv++) {
		c = scanf(*argv, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
		printf("********\n%s\n: ", *argv);
		printf("scanned %d item%s\n", c, c > 1 ? "s" : "");
		for(int i = 0; i < c && i < 6; i++) {
			show_buf(a[i], i);
		}

		ld = ftell(stdin);
		if( ld != -1 ) {
			printf("stream now at position %ld, ", ld);
		}
		c = getchar();
		ungetc(c, stdin);
		printf("next char '%c' ", c);
		printf(c != EOF ? "(%x)" : "(EOF)", c);
		putchar('\n');
		if(feof(stdin) && fseek(stdin, 0L, SEEK_SET) == -1) {
			break;
		}
	}
}

void
show_buf(char *a, int idx)
{
	printf("item (%d): ", idx);
	if( isprint(*a) ) {
		printf("'%c' ", *a);
	}
	a[1023] = '\0';
	if(strlen(a) < 60 && isstring(a)) {
		printf("'%s' ", a);
	} else {
		printf("(%02x%02x%02x%02x), ", a[0], a[1], a[2], a[3]);
	}
	putchar('\n');
}

int
isstring(const char *s)
{
	for( ; *s; s++ ) {
		if( !isprint(*s) && *s != '\n') {
			printf("unprintable: %02x\n", *s);
			return 0;
		}
	}
	return 1;
}

/* Handy wrapper
 * Incredibly fragile (does not match `[]` accurately, etc.  Just
 * designed to work with all the cases given here. )
 */
int
scan(const char *input, const char *fmt, ...)
{
	va_list ap;
	int rv;
	int type;
	const char *e;
	union {
		char *s;
		int *d;
	} buf;

	e = fmt;
match:
	if( (e = strchr(e, '%')) != NULL ) {
		e += 1;
		if( *e == '%' ) {
			e += 1;
			goto match;
		}
		e += strspn(e, ".0123456789");
		if( *e == '[' ) {
			e = strchr(e, ']') + 1; /* HAndle NULL */
		}
		e += strspn(e, ".0123456789");
	} else {
		fprintf(stderr, "Invalid format string: %s\n", fmt);
	}
	type = *e;
	va_start(ap, fmt);
        rv = vsscanf(input, fmt, ap);
	va_end(ap);

	printf("On input '%s' with format '%s' (type %c): ", input, fmt, type);
	va_start(ap, fmt);
	switch(type) {
	case 's':
		buf.s = va_arg(ap, char *);
		printf("'%s'", buf.s);
		break;
	case 'd':
		buf.d = va_arg(ap, int *);
		printf("'%d'", *buf.d);
		break;
	}
	va_end(ap);
	putchar('\n');

	return rv;
}
