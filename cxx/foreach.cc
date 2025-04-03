

#include <iostream>
#include <algorithm>
#include <vector>

std::vector<int> y;

int main()
{
    std::vector<int> x {1, 2, 3};
    std::for_each(x.begin(), x.end(), [](int &n) { y.push_back(n); });
    std::cout << "x\n";
    std::for_each(x.begin(), x.end(), [](int &n) { std::cout << ", " << n;});
    std::cout << "y\n";
    std::for_each(y.begin(), y.end(), [](int &n) { std::cout << ", " << n;});

	return 0;
}
