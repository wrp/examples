
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(){

	int i = 5;
	auto show = [](int i, string n) { cout << n << ": " << i << "  "; };

	/* Capture all variables by reference */
	auto f = [&](){ show(i++, "f"); };

	/* Capture all variables by value */
	auto g = [=]() mutable { show(i++, "g"); };

	/* Capture no variables */
	auto h = [show](int i) { show(i++, "h"); };

	for( ; i < 10; i += 1 ){
		f();
		g();
		h(i);
		cout << '\n';
	}
}
