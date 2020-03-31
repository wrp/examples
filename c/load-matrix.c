#include <stdio.h>
#include "xutil.h"

/* Read text integers into a 2-d array */
struct array {
	int *data;
	int rows;
	int columns;
};

size_t
parse_line(const char *line, int *row, size_t s)
{
	const char *end = line;
	size_t count = 0;
	while(1) {
		line = end;
		int v = strtol(line, (char **)&end, 10);
		if(end == line)
			break;
		if(row != NULL && count < s) {
			*row++ = v;
		}
		count += 1;
	}
	return count;
}


void
push(const char *line, struct array *A)
{
	size_t c;
	A->rows += 1;
	A->data = realloc(A->data, A->rows * A->columns * sizeof *A->data);
	if(A->data == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	c = parse_line(line, A->data + A->columns * (A->rows - 1), A->columns);
	if( c != (size_t)A->columns ) {
		fprintf(stderr, "Invalid input on row %d.  Expected %d "
			"entries, but got %zd\n", A->rows, A->columns, c);
		exit(EXIT_FAILURE);
	}
}

int
main(int argc, char **argv)
{
	char *line = NULL;
	size_t capacity = 0;
	struct array A = { .data = NULL, .rows = 0, .columns = 0 };

	FILE *ifp = argc > 1 ? xfopen(argv[1], "r") : stdin;
	while( getline(&line, &capacity, ifp) != -1 ) {
		fflush(stdout);
		if( A.columns == 0 ) {
			A.columns = parse_line(line, NULL, 0);
		}
		push(line, &A);
	}
	int *row = A.data;
	for(int r=0; r < A.rows; r++, row += A.columns) {
		for(int c=0; c < A.columns; c++) {
			printf("%8d\t", row[c]);
		}
		putchar('\n');
	}

	free(line);
	free(A.data);

	return 0;
}
