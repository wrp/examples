#include <err.h>
#include <stdio.h>
/*
..#...##...###.........#..#..#.
#.###........#..##.#......#...#
#.#.###..#.#..#.#............#.
*/

int
main(void)
{
	char buf[33];
	int x = 0;
	int line = 1;
	int count = 0;
	while( fgets(buf, sizeof buf, stdin) != NULL ){
		if( buf[31] != '\n' ){
			errx(1, "Unexpected input in line %d: %s", line, buf);
		}
		line += 1;
		if( buf[x] == '#' ){
			count += 1;
		}
		x = (x + 3) % 31;
	}
	printf("%d\n", count);
	return 0;
}
