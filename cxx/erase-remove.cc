

#include <iostream>
#include <algorithm>
#include <deque>


template <typename T>
void show_container(const T & x, const std::string & msg);

int main()
{
	std::deque<int> x { 1, 2, 3 };
	show_container(x, "Before removal");

	x.erase(std::remove(x.begin(), x.end(), 2), x.end());

	show_container(x, "After  removal");
	x.push_front(6); x.push_back(6); x.push_front(8);
	show_container(x, "Before remove_if");

	x.erase(
		std::remove_if(
			x.begin(),
			x.end(),
			[](auto i) {
				if (i == 6) { std::println("removing item"); }
				return i != 6;
			}
		),
		x.end()
	);

	show_container(x, "After remove_if");
}


template <typename T>
void
show_container(const T & x, const std::string & msg)
{
	std::print("{}: ", msg);
	for (auto a: x) {
		std::print("{} ", a);
	}
	std::println("");
}
