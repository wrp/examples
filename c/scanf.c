#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

void show_buf(char *a, int);
int isstring(const char *s);

int
main(int argc, char **argv)
{
	int c;
	long ld;
	char a[7][1024];

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
