

#include <assert.h>
#include <string.h>

int
main(int argc, char **argv)
{
	char dest[8] = "";
	strncat(dest, "abcdefg", 2);   /* Writes 3 characters!! */
	assert(0 == strcmp(dest, "ab"));

	dest[0] = '\0';
	strncat(dest, "abcdefg", 6);
	assert(0 == strcmp(dest, "abcdef"));
	return 0;
}
