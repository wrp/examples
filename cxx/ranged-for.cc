

#include <iostream>
#include <vector>

using namespace std;

int main()
{

	vector<vector<double>> x {
		{5.1, 5.2, 5.3},
		{7.2, 7.3, 7.4}
	};
	for( const auto& a : x ){
		for( const auto& b : a ){
			std::cout << b << (&b == &a.back() ? "\n" : ": ");
		}
	}
	return 0;
}
