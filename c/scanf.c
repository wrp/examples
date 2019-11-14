#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
		printf("scanned %d items, ", c);
		printf("first is '%c' (%02x%02x%02x%02x), ",
			a[0], a[0], a[1], a[2], a[3]);
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
