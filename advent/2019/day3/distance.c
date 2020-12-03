#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Sample input
R75,D30,R83,U83,L12,D49,R71,U7,L72
U62,R66,U55,R34,D71,R55,D58,R83
*/

int
trace_path(unsigned *map, unsigned path, size_t S)
{
	int x = S/2, y =S/2;
	int val;
	int c;
	int dir;
	do {
		switch( dir = getchar() ) {
		default: errx(1, "Unexpected input: %c\n", dir);
		case EOF: return 0;
		case 'R': case 'D': case 'U': case 'L':
			;
		}
		scanf("%d", &val);
		if( val < 0 ) {
			errx(1, "Unexpected value %d\n", val);
		}
		for( ; val > 0; val-- ){
			switch( dir ){
			case 'U': y -= 1; break;
			case 'D': y += 1; break;
			case 'L': x -= 1; break;
			case 'R': x += 1; break;
			}
			if( y < 0 || y > (int)S-1 || x < 0 || x > (int)S-1 ){
				errx(1, "out of bounds\n");
			}
			map[S * y + x] |= path;
		}
	} while( (c = getchar()) == ',' );
	return c == '\n';
}

int
dist(int x, int y, size_t S)
{
	return abs(x - (int)S/2) + abs(y - (int)S/2);
}

int
main(int argc, char *argv[])
{
	size_t S = argc > 1 ? strtol(argv[1], NULL, 10) : 512;
	unsigned *map = calloc(S * S, sizeof *map);
	unsigned path = 0x1;
	int min = INT_MAX;

	while( trace_path(map, path, S) ) {
		path <<= 1;
	}
	for( int y = 0; y < (int)S; y++ ) {
		for( int x = 0; x < (int)S; x++ ){
			if( map[S * y + x] > 2 ){
				int d = dist(x, y, S);
				min = d < min ? d : min;
			}
		}
	}
	printf("%d\n", min);

	return 0;
}
