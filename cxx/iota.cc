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

	std::vector<float> f(100);
	float start = 0.1;
	for(float &fv: f) {
		fv = start;
		start += .7;
	}
	std::cout << std::format("{:f} .. {:f}\n", f[0], f[99]) << '\n';
	return 0;
}
