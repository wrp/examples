#include <iostream>
#include <cstring>

using namespace std;

int
main(int argc, char **argv)
{
	for( int i = 1; i < argc; i += 1 ){
		int x = stoi(argv[i]);
		cerr << x << ": " << strerror(x) << endl;
	}
	return 0;
}
