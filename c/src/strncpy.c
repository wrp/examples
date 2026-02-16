

#include <assert.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char dest[32];

	strncpy(dest, "src", 4);
	assert(strcmp(dest, "src") == 0);
	assert(dest[3] == '\0');

	dest[3] = 'x';
	dest[4] = '\0';
	strncpy(dest, "abcde", 3);  /* does not terminate!! */
	assert(strcmp(dest, "abcx") == 0);

	return 0;
}
