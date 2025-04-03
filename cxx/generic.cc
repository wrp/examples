#include <iostream>
#include <vector>

using namespace std;

template <class T>
T foo(T x) {
	return x + (T)1;
}

int main()
{
	cout << foo<float>(1.3) << endl;
	cout << foo<int>(2) << endl;
	return 0;
}
