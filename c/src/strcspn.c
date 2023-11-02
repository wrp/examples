#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	const char *charset = argc > 1 ? argv[1] : ":";
	const char *s = argc > 2 ? argv[2] : "default : string";
	size_t rv = strcspn(s, charset);
	printf("strcspn(\"%s\", \"%s\") = %zd\n", s, charset, rv);
	return 0;
}
