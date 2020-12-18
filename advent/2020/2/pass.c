
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

/*
1-7 q: qqqqxvqrkbqqztlqlzq
1-3 q: cqbm
15-16 h: hhhhhhhhhhhhhhbsh
4-16 x: xvbxswpnvxtnfjrxxx
6-7 v: kbbvnswp
17-18 h: hhhvhhhhhhhhhhhhhh
1-7 w: twftdrb
*/


int
main(void)
{
	char buf[1024];
	char pas[1024];
	int low, high;
	char c;
	while(fgets(buf, sizeof buf, stdin)) {
		if( buf[strlen(buf) - 1] != '\n' ) {
			fprintf(stderr, "short line: %s\n", buf);
			exit(1);
		}
		if( sscanf(buf, "%d-%d %c: %1023s", &low, &high, &c, pas) != 4 ) {
			fprintf(stderr, "bad line: %s\n", buf);
			exit(1);
		}
		int count = 0;
		if( pas[low - 1] == c ) {
			count += 1;
		}
		if( pas[high - 1] == c ) {
			count += 1;
		}
		if( count == 1 ) {
			printf("match: %d <= %d <= %d: %s", low, count, high, buf);
		}
	}
        return 0;
}
