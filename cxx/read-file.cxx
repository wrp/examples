#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void
readFile(ifstream& file)
{
	string word;
	int c = 0;
	while( file >> word) {
		cout << word << endl;
		c++;
	}
	cout << "Read " << c << " words." << endl;
}

int
main2(int argc, char **argv)
{
	string path = argc > 1 ? argv[1] : "input";
	std::ifstream f;

#if 0
	// Set exceptions to be thrown on failure
	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		f.open(path);
	} catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
#endif
	f.open(path);
	if (!f) {
		throw std::system_error(errno, std::system_category(), path);
	}

	readFile(f);
	return 0;
}

int
main(int argc, char **argv)
{
	try {
		main2(argc, argv);
	} catch( std::system_error &e ) {
		std::cerr << e.code().message() << std::endl;
		return 1;
	}
	return 0;
}
