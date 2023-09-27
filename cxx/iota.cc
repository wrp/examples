#include <format>
#include <iostream>
#include <numeric>
#include <vector>

int
main()
{
	std::vector<int> v(100);
	std::iota(v.begin(), v.end(), 0);
	std::cout << std::format("{:d} .. {:d}\n", v[0], v[99]) << '\n';
	return 0;
}
