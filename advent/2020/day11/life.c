
#include <assert.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { floor, empty, occupied, out_of_bounds } typ;
void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
void * xcalloc(size_t count, size_t size);

int
get_type(char *map, int row, int col, int rows, int cols)
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
find_type_in_dir(char *map, int row, int col, int dir, int rows, int cols)
{
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
	if( ( typ = get_type(map, row, col, rows, cols)) == empty ){
		return find_type_in_dir(map, row, col, dir, rows, cols);
	} else {
		return typ;
	}
}

int
count_occ_adjacent(char *map, int row, int col, int rows, int cols)
{
	int count = 0;
	for( int i = 1; i < 9; i++ ){
		count += find_type_in_dir(map, row, col, i, rows, cols) == occupied;
	}
	return count;
}

int
grow(char *map, char *next, int rows, int cols)
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
main(int argc, char **argv)
{
	int rows = 0;
	int cols = -1;
	size_t cap;
	const char *path = argc > 1 ? argv[1] : "stdin";
	FILE *ifp = argc > 1 ? fopen(path, "r") : stdin;
	char *map[2];
	char *end;
	int idx = 0;
	int i = 0;
	int c;
	if( ifp == NULL ){
		perror(path);
		return 1;
	}
	map[0] = xrealloc(NULL, cap = 128, sizeof *map[0], &end);

	while( (c = fgetc(ifp)) != EOF ){
		if( c == '\n' ) {
			if( cols == -1 ){
				cols = end - map[0];
			} else if( cols != end - (map[0] + rows * cols) ){
				errx(1, "invalid input on line %d", rows);
			}
			rows += 1;
		} else {
			if( end > map[0] + cap ){
				map[0] = xrealloc(map[0], cap += 128, 1, &end);
			}
			*end++ = c;
		}
	}
	map[1] = xrealloc(NULL, cap, sizeof *map[1], NULL);
	do {
		idx = !idx;
		for( int i = 0; i < rows; i++ ){
			char *row = map[!idx] + i * cols;
			for( int j = 0; j < cols; j++ ){
				putchar(row[j]);
			}
			putchar('\n');
		}
		putchar('\n');
	} while( grow(map[!idx], map[idx], rows, cols) > 0 );
	i = 0;
	for( char * row = map[idx]; row - map[idx] < rows; row += cols ){
		for( int col = 0; col < cols; col++ ){
			if( row[col] == '#' ){
				i += 1;
			}
		}
	}
	free(map[0]);
	free(map[1]);
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
