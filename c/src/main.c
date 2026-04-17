#include <stdio.h>

int
#ifdef __APPLE__
main(int argc, char **argv, char **envp, char **apple)
#else
main(int argc, char **argv)
#endif
{

#ifdef __APPLE__
	for (char **ap = apple; *ap; ap += 1) {
		printf("%s\n", *ap);
	}
#endif
	return 0;
}
