

#include <iostream>
#include <vector>

using namespace std;

namespace foo {
	namespace bar {
		enum e { one, two, three };
	}
}

int main()
{
	using a = foo::bar::e;
	using foo::bar::e;
	int i = one;
	int j = e::two;
	cout << "i = " << i << j << "\n";
	return 0;
}
