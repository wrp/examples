#include <stdio.h>
#ifndef FOO
#define FOO 1
#endif
#ifndef BAR
#define BAR 2
#endif

#define xstr(s) str(s)
#define str(s) #s   /* stringify s : expands to "s" */

#if FOO == BAR
#define V "="
#else
#define V "!="
#endif

int
main(void)
{
	printf("%s %s %s\n", xstr(FOO), V, xstr(BAR));  /* 1 != 2 */
	printf("%s %s %s\n", str(FOO), V, str(BAR));    /* FOO != BAR */
	printf("%s = %d\n", str(FOO), FOO);             /* FOO = 1 */
	printf("%s = %d\n", str(BAR), BAR);             /* BAR = 2 */
	return 0;
}
