

#include <iostream>
#include <algorithm>
#include <deque>

using namespace std;
void show_deque(const std::deque<int> & x, const string & msg);

bool remove_and_log_6(int i) {
	if( i == 6 ){
		cout << "removing item\n";
		return false;
	}
	return true;
}

int main()
{
	std::deque<int> x { 1, 2, 3 };
	show_deque(x, "Before removal");

	x.erase(std::remove(x.begin(), x.end(), 2));

	show_deque(x, "After  removal");
	x.push_front(6); x.push_back(6); x.push_front(8);
	show_deque(x, "Before remove_if");

	x.erase(std::remove_if(x.begin(), x.end(), remove_and_log_6));

	show_deque(x, "After remove_if");
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
