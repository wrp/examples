#include <stdio.h>
#include <ctype.h>



void
report(int c) {
	printf("'%1$c'(0x%1$02x)\t%2$sprintable", c, isprint(c) ? "is " : "is not ");
	printf(",\t%s space", isspace(c) ? "is" : "is not");
	putchar('\n');
}

int
main(int argc, char **argv)
{
	int c;
	if( argc ) {
		for( argv += 1; *argv; argv += 1 ) {
			for( char *a = *argv; *a; a += 1 ) {
				report(*a);
			}
		}

	} else while( ( c = getchar()) != EOF ) {
		report(c);
	}
	return 0;
}
