#include <iostream>
#include <functional>

class Finalizer {
private:
	std::function<void()> cleanup_func;

public:
	explicit Finalizer(std::function<void()> func)
		: cleanup_func(std::move(func)) {}

	~Finalizer() {
		if (cleanup_func) {
			cleanup_func();
		}
	}

	Finalizer(const Finalizer&) = delete;
	Finalizer& operator=(const Finalizer&) = delete;

	Finalizer(Finalizer&& other) noexcept
		: cleanup_func(std::move(other.cleanup_func))
	{
		other.cleanup_func = nullptr;
	}
};

int main() {
	std::println("Entering scope");
	{
		Finalizer guard(
			[]() { std::println("Finalizer executed!"); }
		);

		std::println("inside scope");
	}

	std::println("outside scope");
	return 0;
}
