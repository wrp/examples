
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int
sum(int *d, int low, int high)
{
	int s = 0;
	for(int i = low; i < high; i++ ){
		s += d[i];
	}
	return s;
}

int
range(int *d, int low, int high)
{
	int max = d[low];
	int min = d[low];
	for(int i = low; i < high; i++ ){
		if( d[i] > max ){
			max = d[i];
		}
		if( d[i] < min ){
			min = d[i];
		}
	}
	return max + min;
}

int
main(void)
{
	int d[1000];
	for( int i = 0; i < 1000; i++ ){
		if( 1 != scanf("%d", d + i) ){
			errx(EXIT_FAILURE, "invalid input");
		}
	}
	for( int i = 0; i < 1000; i++ ){
		for( int j = 0; j < 1000; j++ ){
			if( sum(d, i, j) == 542529149 ){
				printf("%d\n", range(d, i, j));
				return 0;
			}
		}
	}
	return 1;
}
