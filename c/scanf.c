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
	scan("input string\nline 2", "%[^\n]", buf); /* Writes first line */
	scan("input string\nline 2", "%7[^\n]", buf); /* Write 8 chars */
	scan("24", "%d", &k);
}

int
main(int argc, char **argv)
{
	int c;
	long ld;
	char a[7][1024];

	if(argc == 1) {
		simple_examples();
	} else for(argv += 1; *argv; argv++) {
		c = scanf(*argv, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
		printf("(%d) '%s': ", c, *argv);
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
		printf(c != EOF ? "(0x%02x)" : "(EOF)", c);
		putchar('\n');
		if(feof(stdin) && fseek(stdin, 0L, SEEK_SET) == -1) {
			break;
		}
	}
}

void
show_buf(char *a, int idx)
{
	printf("%d: ", idx + 1);
	a[1023] = '\0';
	if(strlen(a) < 60 && isstring(a)) {
		printf("'%s' ", a);
	} else {
		if( isprint(*a) ) {
			printf("'%c' ", *a);
		}
		printf("(%02x%02x%02x%02x), ", a[0], a[1], a[2], a[3]);
	}
	putchar('\t');
}

int
isstring(const char *s)
{
	for( ; *s; s++ ) {
		if( !isprint(*s) && *s != '\n') {
			return 0;
		}
	}
	return 1;
}

static void
pretty_print(const char *s)
{
	while( *s ) {
		switch(*s) {
		case '\n': fputs("\\n", stdout); break;
		default: putchar(*s);
		}
		s += 1;
	}
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
			e = "s";
		}
	} else {
		fprintf(stderr, "Invalid format string: %s\n", fmt);
	}
	type = *e;
	va_start(ap, fmt);
        rv = vsscanf(input, fmt, ap);
	va_end(ap);

	fputs("On input '", stdout);
	pretty_print(input);
	fputs("' with format '", stdout);
	pretty_print(fmt);
	fputs(", scanned ", stdout);

	va_start(ap, fmt);
	switch(type) {
	case 's':
		buf.s = va_arg(ap, char *);
		printf("'%s' (wrote %lu chars)", buf.s, strlen(buf.s) + 1);
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
