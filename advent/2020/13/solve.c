#include <err.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
939
7,13,x,x,59,x,31,19
*/

void * xrealloc(void *buf, size_t num, size_t siz, void *endvp);
int
main(void)
{
	int start_time, least = INT_MAX;
	char buf[32];
	size_t cap = 32;
	int *v = xrealloc(NULL, cap, sizeof *v, NULL);
	int *e = v;
	int best = -1;
	if( scanf("%d ", &start_time) != 1 ){
		errx(1, "Invalid input");
	}
	while( scanf("%31[^,\n]%*[,\n]", buf) == 1 ){
		char *end;
		*e = strtol(buf, &end, 10);
		if( *end == 'x' ){
			*e = -1;
		} else {
			int td = start_time % *e;
			td = *e - td;
			if( td < least ){
				least = td;
				best = *e;
			}
			printf("%5d -> %d\n", *e, td);
		}
		if( ++e == v + cap ){
			v = xrealloc(v, cap += 32, sizeof *v, &e);
		}
	}
	printf("id %d has delay %d for start at %d\n", best, least, start_time);
	printf("product = %d\n", best * least);
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
