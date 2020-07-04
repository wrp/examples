#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "xutil.h"

struct conversion_specifier {
	const char *s;  /* the % */
	const char *e;  /* One past end of specifier */
	const char *flags;
	const char *conversion;
	size_t width;
};

static void show_bufs(const char *fmt, int count, char a[7][1024]);
static int scan(const char *input, const char *fmt, ...);
static int parse_format_string(const char *fmt, struct conversion_specifier *e);
static void validate(const char *, size_t, int , const char *, int);

static void
simple_examples(void)
{
	char buf[128];
	int k[10];
	printf("%-40s%-20s%s\n", "input:", "format string:", "scanned:");
	scan("input string", "%3s", buf); /* Writes 4 chars: 'inp\0' */
	scan("input string", "%7s", buf); /* Writes 6 chars; 'input\0' */
	scan("input string\nline 2", "%[^\n]", buf); /* Writes first line */
	scan("input string\nline 2", "%7[^\n]", buf); /* Write 8 chars */
	scan("24", "%d", k);
	scan("24", "%1d%d", k, k + 1);
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
	validate("...%17dX...", 17, 'd', "", 'X');
	validate("...%17ldX...", 17, 'd', "l", 'X');
	validate("...%7LgX...", 7, 'g', "L", 'X');
	validate("...%7s...", 7, 's', "", '.');
	validate("...%7[^kdj]...", 7, '[', "", '.');
}

void
print_val(const struct conversion_specifier *f, void *p)
{
	float *g = p;
	double *lg = p;
	long double *Lg = p;
	char *s = p;
	int *d = p;
	long *ld = p;
	long long *lld = p;

	switch(*f->conversion) {
	case 's': case '[':
		printf("'%s'", s);
		break;
	case 'd':
		if(*f->flags == 'l') {
			if( f->flags[1] == 'l') {
				printf("%lld", *lld);
			} else {
				printf("%ld", *ld);
			}
		} else {
			printf("%d", *d);
		}
		break;
	case 'g': case 'f': case 'e': case 'G': case 'F': case 'E':
		switch(*f->flags) {
		case 'l': printf("%lg", *lg); break;
		case 'L': printf("%Lg", *Lg); break;
		default : printf("%g", *g);
		}
		break;
	default:
		if( isprint(*s) ) {
			printf("default: '%c' ", *s);
		}
		printf("(%02x%02x%02x%02x), ", s[0], s[1], s[2], s[3]);
		}
}

/* Find the next conversion specifier and parse it.  Return 0
 * if none found */
static int
parse_format_string(const char *fmt, struct conversion_specifier *e)
{
	const char *s = strchr(fmt, '%');
	if( s == NULL ) {
		return 0;
	}
	if( s[1] == '%' ) {
		return parse_format_string(s + 2, e);
	}
	e->s = s;
	e->width = strtol(s + 1, (char **)&e->flags, 10);
	s = e->flags;
	s += strcspn(s, "diouxXaAeEfFgGsScC[pn");
	e->conversion = s;
	if( *s == '[' ) {
		s += strcspn(s, "]");
	}
	if( e->conversion == e->flags )
		e->flags = "";
	e->e = s + 1;
	return 1;
}


static void
show_bufs(const char *fmt, int count, char b[7][1024])
{
	struct conversion_specifier cs;
	int idx=0;

	cs.e = fmt;
	if(count > 7)
		count = 7;

	while(parse_format_string(cs.e, &cs) && idx < count) {
		char *a = b[idx];
		printf("%d: ", idx + 1);
		print_val(&cs, a);
		putchar('\t');
		idx += 1;
	}
}

static void
pretty_print(const char *s, ptrdiff_t width)
{
	const char *e = s;
	putchar('\'');
	width -= 1;
	while( *e ) {
		if(isprint(*e) || isspace(*e)) switch(*e) {
		case '\f': fputs("\\f", stdout); width -= 1; break;
		case '\n': fputs("\\n", stdout); width -= 1; break;
		case '\r': fputs("\\r", stdout); width -= 1; break;
		case '\t': fputs("\\t", stdout); width -= 1; break;
		case '\v': fputs("\\v", stdout); width -= 1; break;
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


/* Handy wrapper
 * Incredibly fragile (does not match `[]` accurately, etc.  Just
 * designed to work with all the cases given here. )
 */
static int
scan(const char *input, const char *fmt, ...)
{
	struct conversion_specifier cs;

	va_list ap;
	int rv;
	int count = 0;
	union {
		char *s;
		int *d;
	} buf;

	va_start(ap, fmt);
        rv = vsscanf(input, fmt, ap);
	va_end(ap);

	pretty_print(input, 40);
	pretty_print(fmt, 20);

	va_start(ap, fmt);

	cs.e = fmt;
	while(parse_format_string(cs.e, &cs)) {
		if( count++ ) {
			fputs(", ", stdout);
		}
		switch(*cs.conversion) {
		case 's': case '[':
			buf.s = va_arg(ap, char *);
			print_val(&cs, buf.s);
			printf(" (wrote %lu chars)", strlen(buf.s) + 1);
			break;
		case 'd':
			buf.d = va_arg(ap, int *);
			printf("%d", *buf.d);
			break;
		}
	}
	va_end(ap);

	putchar('\n');
	assert(count == rv);

	return rv;
}

static void
validate(const char *fmt, size_t width, int conv, const char *flags, int end)
{
	int k;
	struct conversion_specifier cs;

	k = parse_format_string(fmt, &cs);
	if(!k) {
		die("Failed to parse %s\n", fmt);
	}
	if( cs.width != width ) {
		die("%s: Invalid width:  %zu != %zu\n", fmt, cs.width, width);
	}
	if( cs.conversion[0] != conv ) {
		die("%s: Invalid specifier:  '%c' != '%c'\n",
			fmt, *cs.conversion, conv);
	}
	if( strncmp(flags, cs.flags, strlen(flags)) ) {
		die("%s: Invalid flags:  '%s' != '%s'\n", fmt, cs.flags, flags);
	}
	if( end != *cs.e ) {
		die("%s: Invalid end:  '%c' != '%c'\n", fmt, *cs.e, end);
	}
}
