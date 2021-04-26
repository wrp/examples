#include <stdio.h>
#include <wchar.h>
#include "xutil.h"

int
main(int argc, char **argv)
{
	FILE *ifp = xfopen(argc > 1 ? argv[1] : "-", "r");
	wint_t c;

	while( (c = fgetwc(ifp)) != WEOF ){
		fputwc(c, stdout);
	}
	return 0;
}
