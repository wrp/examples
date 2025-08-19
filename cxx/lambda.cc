
#include <iostream>
#include <vector>
#include <string>
#include <format>

using namespace std;

int main(){

	int i = 5;
	int y = 5;

	/* Square brackets define the capture list */
	auto show = [](int i, string n) { cout << n << ": " << i << "  "; };

	/* Capture all variables by reference */
	auto f = [&](){ show(i++, "f"); };

	/* Capture all variables by value */
	auto g = [=]() mutable { show(i++, "g"); };

	/* Capture no variables, just the function */
	auto h = [show](int i) { show(i++, "h"); };

	/* Capture i by value, y by reference */
	auto j = [i, &y, show] { show(i, "j: i");
		show(y++, "j: y");
	};

	for( ; i < 9; i += 1 ){
		f();
		g();
		h(i);
		j();
		cout << std::format("i = {:d}, y = {:d}\n", i, y);
	}
}
