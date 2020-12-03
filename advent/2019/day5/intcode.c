
#include <ctype.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct int_buf {
	int *start, *end;
	size_t cap;
};

void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);

int
get(struct int_buf *v, unsigned idx, int val, int parm)
{
	char buf[64];
	if( parm < 1 ) {
		errx(1, "invalid parm");
	}
	val /= 10;
	for( ; parm; parm -= 1 ){
		val /= 10;
	}
	int len = sprintf(buf, "%d", val);
	if( len && buf[len - 1] == '1' ){
		return idx < v->cap ? v->start[idx] : EOF;
	}
	return idx < v->cap ? v->start[idx] : EOF;
}

void
put(struct int_buf *v, unsigned idx, int val)
{
	while( idx >= v->cap ) {
		v->start = xrealloc(v->start, v->cap += 128, sizeof *v->start,
			&v->end);
	}
	v->start[idx] = val;
	if( v->end < v->start + idx + 1 ){
		v->end = v->start + idx + 1;
	}
}

int
revstr(char *buf, int val)
{
	char t[64];
	int k = sprintf(t, "%d", val);
	int i;
	for( i = 0; k; i++, k-- ){
		buf[i] = t[k - 1];
	}
	return i;
}

int
getparm(int mode, const struct int_buf *v, int idx)
{
	if( idx < 0 || idx > v->end - v->start - 1 ){
		errx(1, "Invalid param: %d", idx);
	}
	int val = v->start[idx];
	if( mode ) {
		return val;
	} else {
		if( val < 0 || val > v->end - v->start - 1 ){
			errx(1, "Invalid param: %d", val);
		}
		return v->start[val];
	}
}

void
putparm(int mode, struct int_buf *v, int val, int parm)
{
	if( mode ) {
		errx(1, "Invalid input: direct mode in put");
	}
	if( parm < 0 || parm > v->end - v->start - 1 ){
		errx(1, "Invalid param: %d", parm);
	}
	put(v, v->start[parm], val);
}


int
main(int argc, char **argv)
{
	struct int_buf v = {0};
	int val;
	int idx = 0;
	FILE *program = argc > 1 ? fopen(argv[1], "r") : stdin;

	while( fscanf(program, "%d", &val) == 1 ) {
		char comma;
		put(&v, idx++, val);
		if( fscanf(program, " %c", &comma) != EOF && comma != ',' ){
			errx(1, "Bad input");
		}
	}
	int cmd = 0;

	while( v.start + cmd < v.end ) {
		val = v.start[cmd++];
		int opcode = val % 100;
		int r;
		char buf[32] = {0};
		char *mode = buf;

		revstr(buf, val / 100);

		switch( opcode ){
		case 1:
			r = getparm(*mode++ == '1', &v, cmd++);
			r += getparm(*mode++ == '1', &v, cmd++);
			break;
		case 2:
			r = getparm(*mode++ == '1', &v, cmd++);
			r *= getparm(*mode++ == '1', &v, cmd++);
			break;
		case 3:
			if( scanf("%d", &r) != 1 ){
				errx(1, "Bad input");
			}
			break;
		case 4:
			r = getparm(*mode++ == '1', &v, cmd++);
			printf("%d\n", r);
			break;
		case 99:
			exit(0);
		default:
			errx(1, "Bad opcode: %d", opcode);
		}
		if( opcode != 4 ){
			putparm(*mode == '1', &v, r, cmd++);
		}
	}
}

void *
xrealloc(void *buf, size_t num, size_t siz, void *endvp)
{
	void **endp = endvp;
	ptrdiff_t offset = endp && *endp ? *endp - buf : 0;
	if( buf ) {
		buf = realloc(buf, num * siz);
	} else {
		buf = calloc(num, siz);
	}
	if( buf == NULL ){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if( endp != NULL ){
		*endp = buf + offset;
		memset(*endp, 0, num * siz - ((char *)*endp - (char *)buf));
	}
	return buf;
}

