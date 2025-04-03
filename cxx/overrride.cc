
#include <format>
#include <iostream>
#include <vector>

using std::cout;
class base {
public:
	virtual void pure_virtual(int x) = 0;
	virtual void b_virtual(int x) {
		cout << std::format("base: {:s}: {:d}\n", __func__, x);
	};
	void non_virtual(int x) {
		cout << std::format("base: {:s}: {:d}\n", __func__, x);
	}
};

class derived : public base {
public:
	void pure_virtual(int x) {
		cout << std::format("derived: {:s}: {:d}\n", __func__, x);
	};
	virtual void b_virtual(int x) {
		cout << std::format("derived: {:s}: {:d}\n", __func__, x);
	};
	void non_virtual(int x) {
		cout << std::format("derived: {:s}: {:d}\n", __func__, x);
	}

};

using namespace std;

int main()
{
	derived d{};
	d.pure_virtual(3);
	d.b_virtual(3);
	d.non_virtual(3);
	return 0;
}
