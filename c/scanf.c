#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <inttypes.h>  /* SCNdMAX, etc. */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "xutil.h"

/* A "conversion specification" is introduced by '%' and consists of
   - an optional assignment-suppressing character '*'
   - an optional decimal integer > 0 which specified "maximum field width"
   - an optional  "length modifier"  (eg, 'l' in %ld)
   - a conversion specifier
*/

struct specification {
	const char *s;  /* the % */
	const char *e;  /* One past end of specifier */
	const char *flags;
	const char *specifier;
	int suppressed;
	size_t width;
};

static void show_bufs(const char *fmt, int count, char a[7][1024]);
static int scan(const char *input, const char *fmt, ...);
static int parse_format_string(const char *fmt, struct specification *e);
static void validate(const char *, size_t, int , const char *, int);

static void
simple_examples(void)
{
	char buf[128];
	char *p;
	int k[10];
	printf("%-40s%-20s%s\n", "input:", "format string:", "scanned:");
	scan("input string", "%3s", buf); /* Writes 4 chars: 'inp\0' */
	scan("input string", "%7s", buf); /* Writes 6 chars; 'input\0' */
	scan("input string\nline 2", "%[^\n]", buf); /* Writes first line */
	scan("input string\nline 2", "%7[^\n]", buf); /* Write 8 chars */
	scan("input stg\nlin", "%3[^\n]%n%7[^\n]%n", buf, k, buf + 3, k + 1);
	scan("24", "%d", k);
	scan("24", "%1d%d", k, k + 1);

	/* %d will be undefined behavior if the input does not fit in
	an int.  Need to add a width specifier.  Note that this means you
	are truncating the range. (eg, if INT_MAX is 2147483647,
	then the format string must be "%9d" to prevent undefined behavior
	on input 8147483647.)
	*/
	k[0] = snprintf(buf, 0, "%d", INT_MAX);
	printf("k = %d\n", k[0]);
	sprintf(buf, "%%%dd", k[0] - 1);
	scan("1234567890123", buf, k);

	/* Dynamic allocation */
	k[0] = sscanf("this is a string", "%m[^s] %s", &p, buf);
	if( k[0] ){
		printf( "scanned %d items: %s and %s\n", k[0], p, buf);
		free(p);
	}
}

#define show(x) printf(#x" = %s\n", x);

int
main(int argc, char **argv)
{
	int c;
	long ld;
	char a[7][1024];

	if( argc > 1 && ! strcmp(argv[0], "formats") ){
	show(SCNdPTR);
	show(SCNdMAX);
	show(SCNd64);
	show(SCNd32);
	show(SCNdLEAST32);
	show(SCNdLEAST64);
	show(SCNdFAST32);
	show(SCNdFAST64);
	}

	if( argc == 1 ) {
		simple_examples();
	} else for( argv += 1; *argv; argv++ ) {
		c = scanf(*argv, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
		printf("%9d: %20s: ", c, *argv);
		show_bufs(*argv, c, a);

		ld = ftell(stdin);
		if( ld != -1 ) {
			printf("stream now at position %ld, ", ld);
		}
		c = getchar();
		ungetc(c, stdin);
		fputs("next:", stdout);
		if( isprint(c) ) {
			printf("'%c'", c);
		} else {
			printf(c != EOF ? "(0x%02x)" : "(EOF)", c);
		}
		putchar('\n');
		if( feof(stdin) && fseek(stdin, 0L, SEEK_SET) == -1 ) {
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
print_val(const struct specification *f, void *p)
{
	float *g = p;
	double *lg = p;
	long double *Lg = p;
	char *s = p;
	int *d = p;
	long *ld = p;
	long long *lld = p;
	unsigned *u = p;
	unsigned long *lu = p;
	unsigned long long *llu = p;

	switch( *f->specifier ){
	case 's': case '[':
		printf("'%s'", s);
		break;
	case 'd': case 'i':
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
	case 'u':
		if(*f->flags == 'l') {
			if( f->flags[1] == 'l') {
				printf("%llu", *llu);
			} else {
				printf("%lu", *lu);
			}
		} else {
			printf("%u", *u);
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

/* Find the next conversion specification and parse it.  Return 0
 * if none found */
static int
parse_format_string(const char *fmt, struct specification *e)
{
	const char *s = strchr(fmt, '%');
	if( s == NULL ) {
		return 0;
	}
	if( s[1] == '%' ) {
		return parse_format_string(s + 2, e);
	}
	e->s = s;
	if( 0 != (e->suppressed = (s[1] == '*')) ){
		s += 1;
	}
	e->width = strtol(s + 1, (char **)&e->flags, 10);
	s = e->flags;
	s += strcspn(s, "diouxXaAeEfFgGsScC[pn");
	e->specifier = s;
	if( *s == '[' ) {
		s += strcspn(s, "]");
	}
	if( e->specifier == e->flags )
		e->flags = "";
	e->e = s + 1;
	return 1;
}


static void
show_bufs(const char *fmt, int count, char b[7][1024])
{
	struct specification cs;
	cs.e = fmt;
	if( count > 7 ){
		count = 7;
	}

	for( int i = 0; parse_format_string(cs.e, &cs) && i < count; ) {
		if( cs.suppressed ){
			continue;
		}
		char *a = b[i];
		printf("%d: ", i + 1);
		print_val(&cs, a);
		putchar('\t');
		i += 1;
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
	struct specification cs;

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
	while( parse_format_string(cs.e, &cs) ) {
		if( cs.suppressed ){
			continue;
		}
		if( count++ ) {
			fputs(", ", stdout);
		}
		switch(*cs.specifier) {
		case 's': case '[':
			buf.s = va_arg(ap, char *);
			print_val(&cs, buf.s);
			printf(" (%lu long)", strlen(buf.s));
			break;
		case 'n':
			count -= 1;
			/* Fall Through */
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
	struct specification cs;

	k = parse_format_string(fmt, &cs);
	if( cs.suppressed ){
		return;
	}
	if( !k ) {
		die("Failed to parse %s\n", fmt);
	}
	if( cs.width != width ) {
		die("%s: Invalid width:  %zu != %zu\n", fmt, cs.width, width);
	}
	if( cs.specifier[0] != conv ) {
		die("%s: Invalid specifier:  '%c' != '%c'\n",
			fmt, *cs.specifier, conv);
	}
	if( strncmp(flags, cs.flags, strlen(flags)) ) {
		die("%s: Invalid flags:  '%s' != '%s'\n", fmt, cs.flags, flags);
	}
	if( end != *cs.e ) {
		die("%s: Invalid end:  '%c' != '%c'\n", fmt, *cs.e, end);
	}
}

#if 0

From n1256.pdf

(7.19.6.2.3) "The format shall be a multibyte character sequence, beginning
and ending in its initial shift state.  The format is composed of zero or
more directives: one or more whitespace characters, an ordinary multibyte
character (neither % nor a whitespace character), or a conversion specification.
Each conversion specification is introduced by the character %.  After the %,
the following appear in sequence:

    - An optional assignment-suppressing character *.
    - An optional decimal integer greater than 0 the specifies the maximum
      field width (in characters).
    - An optional length modifier that specified the size of the receiving
      object.
    - A conversion specifier character the specifies the type of the
      conversion to be applied.

(7.19.6.2.13) "If a conversion specification is invalid, the
behavior is undefined."





SCANF(3)                 BSD Library Functions Manual                 SCANF(3)

NAME
     fscanf, scanf, sscanf, vfscanf, vscanf, vsscanf -- input format conversion

LIBRARY
     Standard C Library (libc, -lc)

SYNOPSIS
     #include <stdio.h>

     int
     fscanf(FILE *restrict stream, const char *restrict format, ...);

     int
     scanf(const char *restrict format, ...);

     int
     sscanf(const char *restrict s, const char *restrict format, ...);

     #include <stdarg.h>
     #include <stdio.h>

     int
     vfscanf(FILE *restrict stream, const char *restrict format, va_list arg);

     int
     vscanf(const char *restrict format, va_list arg);

     int
     vsscanf(const char *restrict s, const char *restrict format, va_list arg);

DESCRIPTION
     The scanf() family of functions scans input according to a format, as described below.  This format may contain conversion
     specifiers; the results from such conversions, if any, are stored through the pointer arguments.  The scanf() function reads
     input from the standard input stream stdin, fscanf() reads input from the stream pointer stream, and sscanf() reads its input
     from the character string pointed to by s.

     The vfscanf() function is analogous to vfprintf(3) and reads input from the stream pointer stream using a variable argument
     list of pointers (see stdarg(3)).  The vscanf() function scans a variable argument list from the standard input and the
     vsscanf() function scans it from a string; these are analogous to the vprintf() and vsprintf() functions, respectively.

     Each successive pointer argument must correspond properly with each successive conversion specifier (but see the * conversion
     below).  All conversions are introduced by the % (percent sign) character.  The format string may also contain other charac-
     ters.  White space (such as blanks, tabs, or newlines) in the format string match any amount of white space, including none, in
     the input.  Everything else matches only itself.  Scanning stops when an input character does not match such a format charac-
     ter.  Scanning also stops when an input conversion cannot be made (see below).

     Extended locale versions of these functions are documented in scanf_l(3).  See xlocale(3) for more information.

CONVERSIONS
     Following the % character introducing a conversion, there may be a number of flag characters, as follows:

     *        Suppresses assignment.  The conversion that follows occurs as usual, but no pointer is used; the result of the conver-
              sion is simply discarded.

     hh       Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a char (rather than int).

     h        Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a short int (rather than
              int).

     l (ell)  Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a long int (rather than
              int), that the conversion will be one of a, e, f, or g and the next pointer is a pointer to double (rather than
              float), or that the conversion will be one of c, s or [ and the next pointer is a pointer to an array of wchar_t
              (rather than char).

     ll (ell ell)
              Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a long long int (rather
              than int).

     L        Indicates that the conversion will be one of a, e, f, or g and the next pointer is a pointer to long double.

     j        Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a intmax_t (rather than
              int).

     t        Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a ptrdiff_t (rather than
              int).

     z        Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a size_t (rather than
              int).

     q        (deprecated.)  Indicates that the conversion will be one of dioux or n and the next pointer is a pointer to a long
              long int (rather than int).

     In addition to these flags, there may be an optional maximum field width, expressed as a decimal integer, between the % and the
     conversion.  If no width is given, a default of ``infinity'' is used (with one exception, below); otherwise at most this many
     bytes are scanned in processing the conversion.  In the case of the lc, ls and l[ conversions, the field width specifies the
     maximum number of multibyte characters that will be scanned.  Before conversion begins, most conversions skip white space; this
     white space is not counted against the field width.

     The following conversions are available:

     %     Matches a literal `%'.  That is, ``%%'' in the format string matches a single input `%' character.  No conversion is
           done, and assignment does not occur.

     d     Matches an optionally signed decimal integer; the next pointer must be a pointer to int.

     i     Matches an optionally signed integer; the next pointer must be a pointer to int.  The integer is read in base 16 if it
           begins with `0x' or `0X', in base 8 if it begins with `0', and in base 10 otherwise.  Only characters that correspond to
           the base are used.

     o     Matches an octal integer; the next pointer must be a pointer to unsigned int.

     u     Matches an optionally signed decimal integer; the next pointer must be a pointer to unsigned int.

     x, X  Matches an optionally signed hexadecimal integer; the next pointer must be a pointer to unsigned int.

     a, A, e, E, f, F, g, G
           Matches a floating-point number in the style of strtod(3).  The next pointer must be a pointer to float (unless l or L is
           specified.)

     s     Matches a sequence of non-white-space characters; the next pointer must be a pointer to char, and the array must be large
           enough to accept all the sequence and the terminating NUL character.  The input string stops at white space or at the
           maximum field width, whichever occurs first.

           If an l qualifier is present, the next pointer must be a pointer to wchar_t, into which the input will be placed after
           conversion by mbrtowc(3).

     S     The same as ls.

     c     Matches a sequence of width count characters (default 1); the next pointer must be a pointer to char, and there must be
           enough room for all the characters (no terminating NUL is added).  The usual skip of leading white space is suppressed.
           To skip white space first, use an explicit space in the format.

           If an l qualifier is present, the next pointer must be a pointer to wchar_t, into which the input will be placed after
           conversion by mbrtowc(3).

     C     The same as lc.

     [     Matches a nonempty sequence of characters from the specified set of accepted characters; the next pointer must be a
           pointer to char, and there must be enough room for all the characters in the string, plus a terminating NUL character.
           The usual skip of leading white space is suppressed.  The string is to be made up of characters in (or not in) a particu-
           lar set; the set is defined by the characters between the open bracket [ character and a close bracket ] character.  The
           set excludes those characters if the first character after the open bracket is a circumflex ^.  To include a close
           bracket in the set, make it the first character after the open bracket or the circumflex; any other position will end the
           set.  The hyphen character - is also special; when placed between two other characters, it adds all intervening charac-
           ters to the set.  To include a hyphen, make it the last character before the final close bracket.  For instance,
           `[^]0-9-]' means the set ``everything except close bracket, zero through nine, and hyphen''.  The string ends with the
           appearance of a character not in the (or, with a circumflex, in) set or when the field width runs out.

           If an l qualifier is present, the next pointer must be a pointer to wchar_t, into which the input will be placed after
           conversion by mbrtowc(3).

     p     Matches a pointer value (as printed by `%p' in printf(3)); the next pointer must be a pointer to void * (or other pointer
           type).

     n     Nothing is expected; instead, the number of characters consumed thus far from the input is stored through the next
           pointer, which must be a pointer to int.  This is not a conversion, although it can be suppressed with the * flag.

     The decimal point character is defined in the program's locale (category LC_NUMERIC).

     For backwards compatibility, a ``conversion'' of `%\0' causes an immediate return of EOF.

RETURN VALUES
     These functions return the number of input items assigned.  This can be fewer than provided for, or even zero, in the event of
     a matching failure.  Zero indicates that, although there was input available, no conversions were assigned; typically this is
     due to an invalid input character, such as an alphabetic character for a `%d' conversion.  The value EOF is returned if an
     input failure occurs before any conversion such as an end-of-file occurs.  If an error or end-of-file occurs after conversion
     has begun, the number of conversions which were successfully completed is returned.

SEE ALSO
     getc(3), mbrtowc(3), printf(3), scanf_l(3), strtod(3), strtol(3), strtoul(3), wscanf(3)

STANDARDS
     The functions fscanf(), scanf(), sscanf(), vfscanf(), vscanf(), and vsscanf() conform to ISO/IEC 9899:1999 (``ISO C99'').

BUGS
     Earlier implementations of fscanf treated %D, %E, %F, %O and %X as their lowercase equivalents with an l modifier.  In addi-
     tion, fscanf treated an unknown conversion character as %d or %D, depending on its case.  This functionality has been removed.

     Numerical strings are truncated to 512 characters; for example, %f and %d are implicitly %512f and %512d.

     The %n$ modifiers for positional arguments are not implemented.

     The fscanf family of functions do not correctly handle multibyte characters in the format argument.

BSD                             January 4, 2003                            BSD


Note from linux manpage:
       To  use the dynamic allocation conversion specifier, specify m as a length modifier (thus %ms or %m[range]).  The caller must free(3) the returned string, as in the
       following example:

           char *p;
           int n;

           errno = 0;
           n = scanf("%m[a-z]", &p);
           if (n == 1) {
               printf("read: %s\n", p);
               free(p);
           } else if (errno != 0) {
               perror("scanf");
           } else {
               fprintf(stderr, "No matching characters\n");
           }

       As shown in the above example, it is necessary to call free(3) only if the scanf() call successfully read a string.
#endif
