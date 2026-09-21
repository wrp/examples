// The erase-remove idiom is obsolete.  std::erase is now available


#include <iostream>
#include <algorithm>
#include <deque>


template <typename T>
void show_container(const T & x, const std::string & msg);

int main()
{
	std::deque<int> x { 1, 2, 3 };
	show_container(x, "Before removal");

	std::erase(x, 2);

	show_container(x, "After  removal");
	x.push_front(6); x.push_back(6); x.push_front(8);
	show_container(x, "Before x.erase");
	x.erase(x.begin() + 1);
	show_container(x, "After x.erase");
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
