
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int
is_valid(int *d, int next)
{
	for( int i = 0; i < 25; i++ ){
		for( int j = 0; j <25; j++ ){
			if( i != j && next == d[i] + d[j] ){
				return 1;
			}
		}
	}
	return 0;
}

int
main(void)
{
	int d[25];
	int next;
	int i;
	for( i = 0; i < 25; i++ ){
		if( 1 != scanf("%d", d + i) ){
			errx(EXIT_FAILURE, "invalid input");
		}
	}
	while( 1 == scanf("%d", &next) ){
		printf("%d\n", next);
		if( is_valid(d, next) ){
			d[i++ % 25] = next;
		} else {
			return 0;
		}
	}
	return 1;
}
