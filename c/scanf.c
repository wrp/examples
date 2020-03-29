#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

void show_bufs(const char *fmt, int count, char a[7][1024]);
int isstring(const char *s);
int scan(const char *input, const char *fmt, ...);
int get_next_type(const char *e, const char **t);

struct conversion_specifier {
	const char *s;  /* the % */
	const char *e;  /* One past end of format string */
	const char *flags;
	const char *conversion;
	size_t width;
};


void
simple_examples(void)
{
	char buf[128] = "XXXXXXXXXXXXx";
	int k;
	printf("%-40s\t%-20s\t%s\n", "input:", "format string:", "scanned:");
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
		printf("(%d) %20s: ", c, *argv);
		show_bufs(*argv, c, a);

		ld = ftell(stdin);
		if( ld != -1 ) {
			printf("stream now at position %ld, ", ld);
		}
		c = getchar();
		ungetc(c, stdin);
		fputs("next:", stdout);
		if(isprint(c)) {
			printf("'%c'", c);
		} else {
			printf(c != EOF ? "(0x%02x)" : "(EOF)", c);
		}
		putchar('\n');
		if(feof(stdin) && fseek(stdin, 0L, SEEK_SET) == -1) {
			break;
		}
	}
}

void
show_bufs(const char *fmt, int count, char b[7][1024])
{
	(void)fmt;
	for(int idx=0; idx < count && idx < 7; idx++) {
		char *a = b[idx];
		const char *e;
		char buf[1024];
		printf("%d: ", idx + 1);
		switch(get_next_type(fmt, &e)) {
		case 's': printf("'%s' ", a); break;
		case 'g': case 'f': case 'e': case 'G': case 'F': case 'E':
			memcpy(buf, fmt, e - fmt);
			buf[ e - fmt] = '\0';
			if(e[-2] == 'l') {
				printf(buf, *(double *)a);
			} else if(e[-2] == 'L') {
				printf(buf, *(long double *)a);
			} else {
				printf(buf, *(float *)a);
			}
			break;
		default:
			if( isprint(*a) ) {
				printf("default: '%c' ", *a);
			}
			printf("(%02x%02x%02x%02x), ", a[0], a[1], a[2], a[3]);
		}
		putchar('\t');
	}
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
pretty_print(const char *s, ptrdiff_t width)
{
	const char *e = s;
	putchar('\'');
	width -= 1;
	while( *e ) {
		if(isprint(*e) || isspace(*e)) switch(*e) {
		case '\f': fputs("\\f", stdout); break;
		case '\n': fputs("\\n", stdout); break;
		case '\r': fputs("\\r", stdout); break;
		case '\t': fputs("\\t", stdout); break;
		case '\v': fputs("\\v", stdout); break;
		default: putchar(*e);
		} else {
			printf("?%02x", *e);
		}
		e += 1;
	}
	putchar('\'');
	width -= 1;
	while( e++ - s < width ) {
		putchar(' ');
	}
}


/* Determine the type of the next format string */
int
get_next_type(const char *e, const char **t)
{
	if( (e = strchr(e, '%')) != NULL ) {
		e += 1;
		if( *e == '%' ) {
			return get_next_type(e+1, t);
		}
		e += strcspn(e, "diouxXaAeEfFgGsScC[pn");
		if( *e == '[' ) {
			e += strcspn(e, "]");
			if(t) *t = e + 1;
			return 's';
		}
		if(t) *t = e + 1;
		return *e;
	} else {
		fprintf(stderr, "Invalid format string: %s\n", e);
	}
	return 0;
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
	union {
		char *s;
		int *d;
	} buf;

	type = get_next_type(fmt, NULL);
	va_start(ap, fmt);
        rv = vsscanf(input, fmt, ap);
	va_end(ap);

	pretty_print(input, 40);
	putchar('\t');
	pretty_print(fmt, 20);
	putchar('\t');

	va_start(ap, fmt);
	switch(type) {
	case 's':
		buf.s = va_arg(ap, char *);
		printf("'%s' (wrote %lu chars)", buf.s, strlen(buf.s) + 1);
		break;
	case 'd':
		buf.d = va_arg(ap, int *);
		printf("%d", *buf.d);
		break;
	}
	va_end(ap);

	putchar('\n');

	return rv;
}
