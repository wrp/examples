/* demonstrate gcc's cleanup attribute */

#include <stdio.h>

void
scoped(int *p)
{
	printf("variable (%p) being cleaned\n", p);
}

int
main(void)
{
	printf("before scope\n");
	{
		int watched __attribute__((cleanup (scoped)));
		printf("in scope: %p\n", &watched);
	}
	printf("after scope\n");
}
