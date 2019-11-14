#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

int isstring(const char *s);
int
main(int argc, char **argv)
{
	int c;
	long ld;
	char a[1024];
	char b[1024];
	char s[1024];

	(void)argc;
	for(argv += 1; *argv; argv++) {
		c = scanf(*argv, a, b, s, s, s, s, s, s, s, s);
		printf("%s\t: ", *argv);
		printf("scanned %d item%s, ", c, c > 1 ? "s" : "");
		printf("first item scanned: ");
		if( isprint(*a) ) {
			printf("'%c' ", *a);
		}
		a[1023] = '\0';
		if(strlen(a) < 60 && isstring(a)) {
			printf("'%s' ", a);
		} else {
			printf("(%02x%02x%02x%02x), ", a[0], a[1], a[2], a[3]);
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

int
isstring(const char *s)
{
	for( ; *s; s++ ) {
		if( !isprint(*s)) {
			return 0;
		}
	}
	return 1;
}
