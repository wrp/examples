/* Sort integers or chars using Generic */

#include <stdio.h>
#include <stdlib.h>

#define expand(T) \
static void \
show_##T(const T *a, size_t s) \
{ \
	while( s-- ){ \
		printf("%d\t", *a++); \
	} \
	putchar('\n'); \
} \
\
static int \
cmp_##T(const void *va, const void *vb) \
{ \
	const T *a = va; \
	const T *b = vb;  \
	return *a > *b ? 1 : *a < *b ? -1 : 0; \
} \
\
static void \
sort_##T(void *base, size_t nel) \
{ \
	qsort(base, nel, sizeof(T), cmp_##T); \
} \

expand(int)
expand(char)
#undef expand

#define sort(a, b) _Generic((a), \
	int *: sort_int, \
	char *: sort_char \
)(a, b)

#define show(a, b) _Generic((a), \
	int *: show_int, \
	char *: show_char \
)(a, b)


int
main(void)
{
	int x[] = { 5, 7, 57463, -956 };
	int y[] = { 5, 7, 37, -95, 27, 84 };

	show(x, sizeof x / sizeof *x);
	show(y, sizeof y / sizeof *y);
	sort(x, sizeof x / sizeof *x);
	sort(y, sizeof y / sizeof *y);
	show(x, sizeof x / sizeof *x);
	show(y, sizeof y / sizeof *y);

/*
	sort(x, sizeof x / sizeof *x, sizeof *x);
	sort(x, sizeof x / sizeof *x, sizeof *x);
	*/


}

