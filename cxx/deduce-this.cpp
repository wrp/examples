#include <utility>
#include <iostream>
#include <string>
#include <cassert>



using Data = std::string;

// Pre C++23, you hade to write separate methods
struct OldContainer {
	Data value;

	Data& get() & { return value; }
	const Data& get() const& { return value; }
	Data&& get() && { return std::move(value); }
	const Data&& get() const&& { return std::move(value); }
};


// In C++23, a single template captures: const, non-const, lvalue, and rvalue
struct ModernContainer {
	Data value = "Hello";

	template <typename Self>
	auto&& get(this Self&& self) {

		// cast 'value' to match the qualifiers of 'self'
		return std::forward_like<Self>(self.value);
	}
};


int
main()
{
	ModernContainer c;
	const ModernContainer cc;

	// 1. Non-const lvalue
	// Self deduces as: ModernContainer&
	// Returns: Data&
	Data& ref1 = c.get();
	assert(ref1 == "Hello");

	// 2. Const lvalue
	// Self deduces as: const ModernContainer&
	// Returns: const Data&
	const Data& ref2 = cc.get();
	assert(ref2 == "Hello");


	// 3. Non-const rvalue
	// Self deduces as: ModernContainer
	// Returns: Data&& (perfect for moving!)
	Data moved = std::move(c).get();
	assert(moved == "Hello");

	// 4. Const rvalue
	// Self deduces as: const ModernContainer
	// Returns: const Data&&
	const Data&& ref3 = std::move(cc).get();
	assert(ref3 == "Hello");
}
