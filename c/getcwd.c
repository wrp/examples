#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	char name[PATH_MAX];
	getcwd(name, sizeof name);
	printf("cwd = %s\n", name);
	return 0;
}
