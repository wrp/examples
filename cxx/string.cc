

#include <iostream>
#include <vector>
#include <format>

using namespace std;

int main()
{
	string foo = "bar";
	/* huh!?  The string class overrides == so this actually works! */
	if( foo == "bar" ){
		std::cout << "yes" << endl;
	}

	int h{10};
	const auto e {std::format("h = {:d}", h)};
	cout << e << endl;


	std::cout << "Enter a string: ";
	std::cin >> foo;
	std::cout << "foo = '" << foo << "'" << endl;
	return 0;
}
