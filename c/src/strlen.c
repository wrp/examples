#include <assert.h>
#include <string.h>


int
main(void)
{
	char hello[] = "world!";

	assert(strlen(hello) == sizeof hello - 1);
	assert(strlen(hello) == 6);
}
