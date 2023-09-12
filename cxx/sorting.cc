
#include <iostream>
#include <ranges>
#include <vector>

struct Task {
	std::string desc;
	int priority { 0 };
};

int main(){
	std::vector<Task> tasks {
		{ "task 0" },
		{ "task 15", 15 },
		{ "task -8", -8 },
		{ "task 4", 4 }
	};
	auto print = [](Task& t) {
		std::cout << t.desc << ", priority: " << t.priority << '\n';
	};
	std::ranges::sort(tasks, std::ranges::greater{}, &Task::priority);
	std::ranges::for_each(tasks, print);

	std::vector<int> u { 4, -1, 2, -9, 1 };

	std::ranges::sort(u);
    std::ranges::for_each(u, [](int i) { std::cout << i << ", "; });
	std::cout << '\n';

	std::ranges::sort(u, {}, [](int i) { return std::abs(i); });
    std::ranges::for_each(u, [](int i) { std::cout << i << ", "; });
	std::cout << '\n';
}
