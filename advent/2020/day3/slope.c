#include <err.h>
#include <stdio.h>
#include <stdlib.h>
/*
..#...##...###.........#..#..#.
#.###........#..##.#......#...#
#.#.###..#.#..#.#............#.
*/

int
main(int argc, char **argv)
{
	int right = argc > 1 ? strtol(argv[1], NULL, 0) : 3;
	int down = argc > 2 ? strtol(argv[2], NULL, 0) : 1;
	char buf[33];
	int x = 0;
	int line = 0;
	int count = 0;
	while( fgets(buf, sizeof buf, stdin) != NULL ){
		if( line++ % down ){
			continue;
		}
		if( buf[31] != '\n' ){
			errx(1, "Unexpected input in line %d: %s", line, buf);
		}
		if( buf[x] == '#' ){
			count += 1;
		}
		x = (x + right) % 31;
	}
	printf("%d\n", count);
	return 0;
}
