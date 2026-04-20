
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Foo {
public:
	int i;
	Foo(int x) : i{x} { cout << "regular ctor: " << *this << "\n"; }
	void trace(const char *label, const Foo &from) {
		cout << label << ": " << *this << " <-- " << from << '\n';
	}
	Foo(Foo&& other) : i{other.i} { trace("move ctor", other); }
	Foo& operator=(Foo&& other) {
		trace("move op", other);
		i = other.i;
		return *this;
	}
	Foo& operator=(const Foo& other) {
		trace("copy op", other);
		i = other.i;
		return *this;
	}
	friend ostream& operator<<(ostream &os, const Foo &f) {
		return os << f.i;
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
		cout << *f << '\n';
	}


	Foo c(3);
	Foo d(5);
	Foo e = std::move(c);   /* move assignment ctor */
	c = e;   /* copy assignment operator */

	return 0;
}
