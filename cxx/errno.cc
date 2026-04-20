#include <iostream>
#include <cstring>

using namespace std;

int
main(int argc, char **argv)
{
	for (int i = 1; i < argc; i += 1) {
		try {
			int x = stoi(argv[i]);
			cerr << x << ": " << strerror(x) << endl;
		} catch (const std::invalid_argument &e) { /* eg "abc" */
			cerr << argv[i] << ": " << e.what() << endl;
		} catch (const std::out_of_range &e) { /* eg "99999999999" */
			cerr << argv[i] << ": " << e.what() << endl;
		}
	}
	return 0;
}
