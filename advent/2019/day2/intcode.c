
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
1,12,2,3,1,1,2,3, 1,3,4,3,1,5,0,3,
2,1,9,19,1,13,19,23, 2,23,9,27,1,6,27,31,
2,10,31,35,1,6,35,39, 2,9,39,43,1,5,43,47,
2,47,13,51,2,51,10,55, 1,55,5,59,1,59,9,63,
1,63,9,67,2,6,67,71,1,5,71,75,1,75,6,79,1,6,
79,83,1,83,9,87,2,87,10,91,2,91,10,95,1,95,
5,99,1,99,13,103,2,103,9,107,1,6,107,111,
1,111,5,115,1,115,2,119,1,5,119,0,99,2,0,14,0
*/

int main(int argc, char **argv)
{
	int v[200];
	int * cmd = v;
	int i;

	for( i = 0; i < 200; i++ ) {
		if( scanf("%d", v + i) != 1 ) {
			fprintf(stderr, "Bad input at %d\n", i);
			exit(1);
		}
		if( getchar() != ',' ) {
			break;
		}
	}
	v[1] = argc > 1 ? strtol(argv[1], NULL, 10) : 12;
	v[2] = argc > 2 ? strtol(argv[2], NULL, 10) : 2;
	while( *cmd != 99 ) {
		int r;
		switch( *cmd ) {
		case 1: r = v[cmd[1]] + v[cmd[2]]; break;
		case 2: r = v[cmd[1]] * v[cmd[2]]; break;
		default:
		fprintf(stderr, "Bad opcode\n");
		exit(1);
		}
		if( cmd[3] < 0 || cmd[3] > 127 ) {
			fprintf(stderr, "Bad index\n");
			exit(1);
		}
		v[cmd[3]] = r;
		cmd += 4;
	}
	printf("%d\n", v[0]);
}
