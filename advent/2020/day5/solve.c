#include <stdio.h>
#include <err.h>

int
compute_id(const char *b)
{
	unsigned row, col;
	int i;
	for( i = 0, row = 0; i < 7; i++ ){
		row <<= 1;
		if( b[i] == 'B' ){
			row = row | 0x1;
		}
	}
	for( col = 0; i < 10; i++ ){
		col <<= 1;
		if( b[i] == 'R' ){
			col = col | 0x1;
		}
	}
	return row * 8 + col;
}


int
main(void)
{
	int taken [960]; /* Max id is 959 */
	char buf[12];
	int max = 0, id;
	while( fgets(buf, sizeof buf, stdin) ){
		if( buf[10] != '\n' ){
			errx(1, "Invalid input");
		}
		id = compute_id(buf);
		max = id > max ? id : max;
		if( id > 959 ){
			errx(1, "Invalid input");
		}
		taken[id] = 1;
	}
	printf("max: %d\n", max);
	for(int i = 0; i < 960; i++ ){
		if( ! taken[i] ){
			printf("free: %d\n", i);
		}
	}
}
