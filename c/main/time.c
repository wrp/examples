
#include <time.h>
#include <stdio.h>

int
main(void)
{
	time_t t;
	time(&t);
	printf("%s\n", ctime(&t));
	return 0;
}
