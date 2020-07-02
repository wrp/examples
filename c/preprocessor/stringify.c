#include <stdio.h>
#ifndef FOO
#define FOO 1
#endif
#ifndef BAR
#define BAR 2
#endif

#define xstr(s) str(s)
#define str(s) #s

#if FOO == BAR
#define V "="
#else
#define V "!="
#endif

int
main(void)
{
	printf("%s %s %s\n", xstr(FOO), V, xstr(BAR));
	printf("%s %s %s\n", str(FOO), V, str(BAR));
	printf("%s = %d\n", str(FOO), FOO);
	printf("%s = %d\n", str(BAR), BAR);
	return 0;
}
