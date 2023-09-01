

#include <iostream>
#include <algorithm>
#include <deque>

using namespace std;
void show_deque(const std::deque<int> & x, const string & msg);

int main()
{
	std::deque<int> x { 1, 2, 3 };
	show_deque(x, "Before removal");

	x.erase(std::remove(x.begin(), x.end(), 2));

	show_deque(x, "After  removal");
}


void
show_deque(const std::deque<int> & x, const string & msg)
{
	cout << msg << ": ";
	for( auto a: x ){
		cout << a << ' ';
	}
	cout << endl;
}
