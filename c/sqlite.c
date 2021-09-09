
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

/* https://www.sqlite.org/cintro.html */


int
foo(void *f, int x, char **a, char **b)
{
	printf("foo called with %d\n", x);
}
int
main(int argc, char **argv)
{
	const char *path = argc > 1 ? argv[1] : "sample-sqlite";
	sqlite3 *s;
	char *err;
	int exist;


	exist = !access(path, R_OK);
	sqlite3_open(path, &s);

	if( exist ){
		sqlite3_exec( s, "blay", foo, NULL, &err);
	} else {
		sqlite3_exec( s, "blob", foo, NULL, &err);
	}
	printf("err = %s\n", err);
}

