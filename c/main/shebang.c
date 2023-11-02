#include <stdio.h>

/*
 When a file has a shebang:

 #!path1 arg1 arg2

and is invoked with "path2 arg3 arg4",
the file at path1 will be executed with arguments:

path1 "arg1 arg2" path2 arg3 arg4

(if both arg1 and arg2 do not exist, a null argument is *not* passed)

 */
int
main(int argc, char **argv)
{
	printf("argc = %d\n", argc);
	for( ; *argv; argv++ ){
		printf("arg: %s\n", *argv);
	}
}
