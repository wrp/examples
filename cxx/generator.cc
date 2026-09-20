
#include <iostream>
#include <coroutine>
#include <exception>

struct Generator {
	struct promise_type {
		int current_value;

		// Called when the coroutine is first invoked to create the
		//  return object.
		Generator get_return_object() {
			return Generator{std::coroutine_handle<promise_type>
				::from_promise(*this)};
		}

		// Dictates whether the coroutine starts suspended (lazy) or
		// running (eager).
		std::suspend_always initial_suspend() { return {}; }

		// Dictates suspension behavior after the coroutine ends.
		std::suspend_always final_suspend() noexcept { return {}; }

		// Handles when 'co_yield' is called inside the coroutine.
		std::suspend_always yield_value(int value) {
			current_value = value;

			// Suspend the coroutine and return to the caller.
			return {};
		}

		// handle return statements or dropping off the end.
		void return_void() {}

		// Handles unhandled exceptions thrown inside the coroutine.
		void unhandled_exception() { std::terminate(); }
	};

	std::coroutine_handle<promise_type> handle;

	// RAII management for the coroutine state frame allocation
	Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
	~Generator() { if (handle) handle.destroy(); }

	// Move-only semantics since the handle unique owns the coroutine frame
	Generator(const Generator&) = delete;
	Generator& operator=(const Generator&) = delete;
	Generator(Generator&& other) noexcept : handle(other.handle) {
		other.handle = nullptr;
	}

	// Resume the coroutine to calculate the next value
	bool next() {
		if (!handle || handle.done()) {
			return false;
		}
		handle.resume();
		return !handle.done();
	}

	// Retrieve the current value yielded by the coroutine
	int value() const {
		return handle.promise().current_value;
	}
};

// The actual coroutine function using 'co_yield'
Generator count_up_to(int max_value) {
	for (int i = 1; i <= max_value; i += 1) {
		std::cout << "[Coroutine] Yielding " << i << "\n";
		co_yield i; // Pause execution and pass control back to main()
	}
}

int main() {
	// The coroutine is created but suspended immediately
	Generator sequence = count_up_to(3);

	std::cout << "[Main] Entering loop to extract values:\n";
	while (sequence.next()) {
		std::cout << "[Main] Received: " << sequence.value() << "\n";
	}

	return 0;
}
