

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int main()
{
    // tuple <int, int> x = make_tuple(3, 5);
    auto x = make_tuple(3, 5);

    // requires -std=c++17
    auto [a, b, c] = make_tuple(1, 2, 3);

    cout << std::get<1>(x) << a << '\n';

	return 0;
}
