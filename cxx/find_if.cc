

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


struct Foo {
	int x;
	Foo(int x) : x{x} { cerr << "ctor: " << x << endl; };
	Foo(const Foo &f) : x{f.x} { cerr << "cp: " << f.x << endl; };
	Foo(const Foo &&f) { cerr << "mv: " << f.x; };
	~Foo() { cerr << "destroy: " << x << endl; }
	friend ostream& operator<<(ostream& os, const Foo& f);
	bool operator ==(const int x) { return this->x == x; }
	bool operator !=(const int x) { return this->x != x; }
	Foo& operator=(const Foo&) = default;
};

template <typename T>
void show_container(const T & x, const string & msg);

ostream& operator<<(ostream& os, const Foo& f)
{
	os << f.x;
	return os;
}


int main()
{
	vector<Foo> x;
	x.reserve(20);
	for( int i = 1; i < 4; ++i ){
		x.emplace_back(i);
	}
	show_container(x, "Container content");

	auto it = find_if(
		x.begin(),
		x.end(),
		[](auto const &i) { return i.x % 2 == 0; }
	);
	if( it != std::end(x) ){
		cout << "found: " << *it << endl;
	}
}


template <typename T>
void
show_container(const T& x, const string & msg)
{
	cerr << msg << ": ";
	for( auto const &a: x ){
		cerr << a << ' ';
	}
	cerr << endl << std::flush;
}
