#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

void
demo(const string &s)
{
	size_t pos;
	try {
		int x = stoi(s, &pos);
		cerr << "\"" << s << "\" -> " << x;
		if (pos < s.size()) {
			cerr << " (trailing: \"" << s.substr(pos) << "\")";
		}
		cerr << endl;
	} catch (const std::invalid_argument &e) {
		cerr << "\"" << s << "\": invalid_argument: " << e.what() << endl;
	} catch (const std::out_of_range &e) {
		cerr << "\"" << s << "\": out_of_range: " << e.what() << endl;
	}
}

int
main(void)
{
	demo("42");
	demo("  -7");
	demo("123abc");
	demo("0xff");
	demo("abc");
	demo("");
	demo("99999999999");
	demo("-99999999999");
	return 0;
}
