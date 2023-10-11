

#include <iostream>
#include <vector>
#include <memory>

using namespace std;


class A {
public:
	int x;
	A(int m) : x{m}{
		cout << "create: " << x << '\n';
	};
	~A() {
		cout << "destroy: " << x << '\n';
	}
};
class B { public: std::shared_ptr<A> a{nullptr}; };

B create_b() {
	cout << "Begin create_b\n";
	B b{};
	b.a = std::make_shared<A>(5);
	cout << "End create_b\n";
	return b;
}

int main() {
	cout << "start main\n";
	B b = create_b();
	cout << "end main\n";
	return 0;
}
