/* demonstrate gcc's cleanup attribute */

#include <stdio.h>
#include <setjmp.h>

void
clean(char **p)
{
	printf("variable %p (%s) destructor\n", p, *p);
}

	jmp_buf j;
void
foo(void)
{
	char *a __attribute__((cleanup (clean))) = "a in foo";
	printf("before scope\n");
	{
		char *b __attribute__((cleanup (clean))) = "b";
		printf("inner scope: %p\n", &a);
	}
	printf("after scope\n");
	/* with the longjmp, clean() is not called */
	longjmp(j, 1);
}

int
main(void)
{
	if( setjmp(j) == 0 ){
		foo();
	}
	return 0;
}
