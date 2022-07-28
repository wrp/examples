#include <iostream>
#include <fstream>
#include <string>


int
main(int argc, char **argv)
{
	const char *path = argc > 1 ? argv[1] : "foo";
	std::ifstream f(path);
	if( ! f.is_open() ){
		std::perror(path);
	}
	return 0;
}
