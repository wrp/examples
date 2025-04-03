

#include <iostream>
#include <algorithm>
#include <deque>

using namespace std;

template <typename T>
void show_container(const T & x, const string & msg);


int main()
{
	std::deque<int> x { 1, 2, 3 };
	show_container(x, "Before removal");

	x.erase(std::remove(x.begin(), x.end(), 2));

	show_container(x, "After  removal");
	x.push_front(6); x.push_back(6); x.push_front(8);
	show_container(x, "Before remove_if");

	x.erase(
		std::remove_if(
			x.begin(),
			x.end(),
			[](auto i) {
				if( i == 6 ){ cout << "removing item\n"; }
				return i != 6;
			}
		)
	);

	show_container(x, "After remove_if");
}


template <typename T>
void
show_container(const T & x, const string & msg)
{
	cout << msg << ": ";
	for( auto a: x ){
		cout << a << ' ';
	}
	cout << endl;
}
