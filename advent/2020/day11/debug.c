
#include <assert.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum typ { floor, empty, occupied, out_of_bounds };
void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
void * xcalloc(size_t count, size_t size);

int
get_type(const char *map, int row, int col, int rows, int cols)
{
	if( row < 0 || row > rows - 1 || col < 0 || col > cols - 1 ){
		return out_of_bounds;
	} else {
		char c = map[row * rows + col];
		switch( c ){
		case 'L': return empty;
		case '#': return occupied;
		case '.': return floor;
		default: assert(0);
		}
	}
}

int
find_type_in_dir(const char *map, int row, int col, int dir, int rows, int cols)
{
	enum typ t;
	switch( dir ){
	case 1: row -= 1; col -= 1; break;
	case 2: row -= 1; col += 0; break;
	case 3: row -= 1; col += 1; break;
	case 4: row += 0; col += 1; break;
	case 5: row += 1; col += 1; break;
	case 6: row += 1; col += 0; break;
	case 7: row += 1; col -= 1; break;
	case 8: row += 0; col -= 1; break;
	}
	if( ( t = get_type(map, row, col, rows, cols)) == empty ){
		return find_type_in_dir(map, row, col, dir, rows, cols);
	} else {
		return t;
	}
}

int
count_occ_adjacent(const char *map, int row, int col, int rows, int cols)
{
	int count = 0;
	for( int i = 1; i < 9; i++ ){
		count += find_type_in_dir(map, row, col, i, rows, cols) == occupied;
	}
	return count;
}

int
grow(const char *map, char *next, int rows, int cols)
{
	int change = 0;
	int i = 0;

	for( char *r = next; r < next + rows * cols; i++, r += cols ){
		for( int col = 0; col < cols; col++ ){
			int count = count_occ_adjacent(map, i, col, rows, cols);
			switch( get_type(map, i, col, rows, cols) ){
			case occupied:
				r[col] = count > 4 ? 'L' : '#';
				break;
			case empty:
				r[col] = count > 0 ? 'L' : '#';
				break;
			case floor:
				r[col] = '.';
				break;
			case out_of_bounds:
				assert( 0 );
			}
			if( r[col] != map[ i * rows + col] ){
				change += 1;
			}
		}
	}
	return change;
}

int
main()
{
	int rows = 0;
	int cols = -1;
	char one[128];
	char two[128];
	char *map[2] = {one, two};
	char *end = one;
	int idx = 0;
	int i = 0;
	char *input = "LL\n##\n";

	for( size_t i = 0; i < strlen(input); i++ ){
		int c = input[i];
		if( c == '\n' ) {
			if( cols == -1 ){
				cols = end - map[0];
			}
			if( cols != end - (map[0] + rows * cols) ){
				errx(1, "invalid input on line %d", rows);
			}
			rows += 1;
		} else {
			*end++ = c;
		}
	}
	do {
		idx = !idx;
		#if 0
		for( int i = 0; i < rows; i++ ){
			char *row = map[!idx] + i * cols;
			for( int j = 0; j < cols; j++ ){
				putchar(row[j]);
			}
			putchar('\n');
		}
		putchar('\n');
		#endif
	} while( grow(map[!idx], map[idx], rows, cols) > 0 );
	i = 0;
	for( char * row = map[idx]; row - map[idx] < rows; row += cols ){
		for( int col = 0; col < cols; col++ ){
			if( row[col] == '#' ){
				i += 1;
			}
		}
	}
	printf("%d\n", i);

	return 0;
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	void **endp = endvp;
	ptrdiff_t offset = endp && *endp ? *endp - buf : 0;
	buf = realloc(buf, num * siz);
	if( buf == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ){
		*endp = buf + offset;
	}
	return buf;
}

void *
xcalloc(size_t count, size_t size)
{
	void *r = calloc(count, size);
	if( r == NULL ){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	return r;
}
