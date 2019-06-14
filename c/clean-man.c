

/*
 * Discard octet before 0x08 to clean up man pages
 */
#include <stdio.h>
#include <assert.h>

int
main( void )
{
	int c;

	int prev = 0x08;
	assert( EOF != 0x08 );
	do {
		c = getchar();
		if( c != 0x08 && prev != 0x08 )
			putchar(prev);
		prev = c;
	} while( c != EOF );
}
