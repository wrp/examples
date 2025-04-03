
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Foo {
public:
	int i;
	Foo(int x) : i{x} { cout << "regular ctor: " << i << "\n";}
	Foo(Foo&& other) {
		cout << "move constructor: " << i << "<--" << other.i << '\n';
	}
	Foo& operator=(Foo&& other) {
		cout << "move operator: " << i << "<--" << other.i << '\n';
		return other;
	}

};


int
main()
{
	vector<unique_ptr<Foo>> x{};

	unique_ptr<Foo> a = make_unique<Foo>(7);
	unique_ptr<Foo> b = make_unique<Foo>(9);

	x.push_back(std::move(a));
	x.push_back(std::move(b));

	x.emplace_back(make_unique<Foo>(11));

	for( const auto &f: x ){
		cout << f->i << '\n';
	}
	return 0;
}
