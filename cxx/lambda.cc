
#include <iostream>
#include <vector>

using namespace std;

int main(){

	int i = 5;

	/* Capture all varaibles by reference */
	auto f = [&](){ cout << "f: " << i << ", "; };

	/* Capture all variables by value, invoke and assign the returned value */
	auto g = [=](){ return i; }();

	/* Capture no variables */
	auto h = [](int j) { cout << "h: " << j << '\n'; };

	for( i = 0; i < 5; i += 1 ){
		f();
		cout << "g: " << g << ", ";
		h(i);
	}
}
