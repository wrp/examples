
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char a[] = { 0xff, 0x7f};

	for( unsigned i = 0; i < sizeof a / sizeof *a; i++ ){
		char *b = a + i;
		printf(" i = %d\n", i);
		printf("no cast       %%02x: %02x\n", *b);
		printf("unsigned char %%02x: %02x\n", (unsigned char)*b);
		printf("no cast         %%d: %d\n", *b);
		printf("unsigned char   %%d: %d\n", (unsigned char)*b);
		printf("unsigned        %%d: %d\n", (unsigned )*b);
	}
}
