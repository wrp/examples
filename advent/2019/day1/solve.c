

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int k;
	int sum = 0;
	int count = 0;
	while( scanf("%d", &k) == 1 ){
		count += 1;
		k = k / 3 - 2;
		sum += k;
	}
	printf("%d\n", sum);
	if( count != 100 ){
		fprintf(stderr, "total lines: %d\n", count);
		return 1;
	}
	return 0;
}
