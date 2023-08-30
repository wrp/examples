

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int main()
{
    // tuple <int, int> x = make_tuple(3, 5);
    auto x = make_tuple(3, 5);
    cout << std::get<1>(x) << '\n';
	return 0;
}
