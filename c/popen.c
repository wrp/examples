/* Simple demonstration of popen */
#include <stdio.h>

int
main(int argc, char *argv[])
{
	const char *cmd = argc > 1 ? argv[1] : "echo foo bar baz";
	char buf[32];
	FILE *fp = popen(cmd, "r");
	if( fp == NULL ){
		perror("popen");
		return 1;
	}
	if( 1 == fscanf(fp, "%*s%31s%*s", buf) ){
		printf("read: %s\n", buf);
	}
	return 0;
}
