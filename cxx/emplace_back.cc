
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Foo {
public:
	int i;
	Foo(int x) : i{x} { cout << "regular ctor: " << i << "\n";}
	Foo(Foo&& other) {
		cout << "move ctor: " << this->i << "<--" << other.i << '\n';
	}
	Foo& operator=(Foo&& other) {
		cout << "move op: " << this->i << "<--" <<  other.i << '\n';
		return *this;
	}
	Foo& operator=(const Foo& other) {
		cout << "copy assignment operator " <<
			this->i << "<--" <<  other.i << '\n';
		i = other.i;
		return *this;
	}
};


int
main()
{
	vector<unique_ptr<Foo>> x{};

	unique_ptr<Foo> a = make_unique<Foo>(7);
	Foo *bp = new Foo(9);
	auto b = std::unique_ptr<Foo>(bp);

	x.push_back(std::move(a));  // Does not use move ctor
	x.push_back(std::move(b));
	x.emplace_back(make_unique<Foo>(11));

	for (const auto &f: x) {
		cout << f->i << '\n';
	}


	Foo c(3);
	Foo d(5);
	Foo e = std::move(c);   /* move assignment ctor */
	c = e;   /* copy assignment operator */

	return 0;
}
