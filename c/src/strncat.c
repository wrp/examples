

#include <assert.h>
#include <string.h>

int
main(void)
{
	char dest[8] = "";
	strncat(dest, "abcdefg", 2);   /* Writes 3 characters!! */
	assert(0 == strcmp(dest, "ab"));

	dest[0] = '\0';
	strncat(dest, "abcdefg", 6);
	assert(0 == strcmp(dest, "abcdef"));

	dest[0] = '\0';
	strncat(dest, "XXXXXXX", 7);
	dest[0] = '\0';
	strncat(dest, "ab", 3);

	assert(dest[2] == '\0');
	assert(dest[3] == 'X');
	assert(dest[4] == 'X');


	return 0;
}
