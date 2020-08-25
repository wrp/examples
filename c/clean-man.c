

/*
 * Clean up man pages.
 * Discard octet before 0x08.
 * Discard all between 0x1b and 'm'
 */
#include <stdio.h>
#include <assert.h>

int
main( void )
{
	int c;

	int prev = 0x08;
	int state = 1;
	assert( EOF != 0x08 );
	do {
		c = getchar();
		if( prev == 0x1b ) {
			state = 0;
		}
		if( state && c != 0x08 && prev != 0x08 )
			putchar(prev);
		if( prev == 'm' ) {
			state = 1;
		}
		prev = c;
	} while( c != EOF );
}
