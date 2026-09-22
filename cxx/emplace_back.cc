
#include <iostream>
#include <vector>
#include <memory>

class Foo {
public:
	int i;
	Foo(int x) : i{x} { std::println("regular ctor: {}", i); }
	void trace(const char *label, const Foo &from) {
		std::println("{}: {} <-- {}", label, i, from.i);
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
	friend std::ostream& operator<<(std::ostream &os, const Foo &f) {
		return os << f.i;
	}
	// The copy ctor is implicitly deleted because there is a move ctor
	Foo(const Foo& other) : i{other.i} { trace("copy ctor", other); }

};


int
main()
{
	std::vector<std::unique_ptr<Foo>> x{};
	std::vector<Foo> y{};

	std::unique_ptr<Foo> a = std::make_unique<Foo>(7);
	Foo bp{Foo(9)};
	auto b = std::unique_ptr<Foo>(&bp);

	x.push_back(std::move(a));  // Does not use move ctor
	x.push_back(std::move(b));
	x.emplace_back(std::make_unique<Foo>(11));

	for (const auto &f: x) {
		std::println("{}", f->i);
	}


	Foo c(3);
	Foo d(5);
	Foo e = std::move(c);   /* move assignment ctor */
	c = e;   /* copy assignment operator */

	std::println("pushing 13");
	y.emplace_back(13);  /* regular ctor */
	std::println("pushing c");
	y.emplace_back(c);  // Causes reallocation, copy ctor called for all

	return 0;
}
