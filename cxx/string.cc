

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	string foo = "bar";
	/* huh!?  The string class overrides == so this actually works! */
	if( foo == "bar" ){
		std::cout << "yes" << endl;
	}
	return 0;
}
