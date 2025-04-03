

#include <iostream>
#include <vector>

using namespace std;

// This program will produce mangled names. To see english
// names, pipe to c++filt --types
int main()
{
    auto m = "test";
    cout << "M is of type: " << typeid(m).name() << '\n';
	return 0;
}
