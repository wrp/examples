
#include <stdio.h>
#include <string.h>
#include <assert.h>

enum { floor, empty, occupied } typ;

int
get_type(char map[100][100], int row, int col)
{
	if( row < 0 || row > 94 || col < 0 || col > 92 ){
		return floor;
	} else switch( map[row][col] ){
		case 'L': return empty;
		case '#': return occupied;
		case '.': return floor;
		default: assert(0);
	}
}

int
count_occ_adjacent(char map[100][100], int row, int col)
{
	int count = 0;
	for( int i = -1; i < 2; i++ ){
		count += get_type(map, row - 1, col + i) == occupied;
		if( i ){
			count += get_type(map, row, col + i) == occupied;
		}
		count += get_type(map, row + 1, col + i) == occupied;
	}
	return count;
}

int
grow(char map[100][100], char next[100][100])
{
	int change = 0;

	for( int row = 0; row < 95; row++ ){
		for( int col = 0; col < 93; col++ ){
			int count = count_occ_adjacent(map, row, col);
			switch( get_type(map, row, col) ){
			case occupied:
				next[row][col] = count > 3 ? 'L' : '#';
				break;
			case empty:
				next[row][col] = count > 0 ? 'L' : '#';
				break;
			case floor:
				next[row][col] = '.';
			}
			if( next[row][col] != map[row][col] ){
				change += 1;
			}
		}
	}
	return change;
}

int
main(int argc, char **argv)
{
	const char *path = argc > 1 ? argv[1] : "stdin";
	FILE *ifp = argc > 1 ? fopen(path, "r") : stdin;
	char map[2][100][100];
	int idx = 0;
	int i = 0;
	int len = 93;
	if( ifp == NULL ){
		perror("path");
		return 1;
	}
	while( fgets(map[idx][i], 100, ifp) ){
		assert( (int)strlen(map[idx][i]) == len + 1);
		assert( map[idx][i][len] == '\n' );
		map[idx][i][len] = 0;
		i += 1;
	}
	assert( i == 95 );
	printf("%d x %d\n", i, (int)strlen(map[0][0]));
	printf("%s\n", map[idx][0]);
	printf("%s\n", map[idx][i - 1]);
	while( (grow(map[idx], map[!idx])) > 0 ){
		idx = !idx;
	}
	i = 0;
	for( int row = 0; row < 95; row++ ){
		for( int col = 0; col < 93; col++ ){
			if( map[0][row][col] == '#' ){
				i += 1;
			}
		}
	}
	printf("%d\n", i);

	return 0;

}
