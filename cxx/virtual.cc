
#include <iostream>
#include <format>
using namespace std;

class Base {
public:
	int x;
	Base(int a) : x{a + 3} {};
	void NonVirtual() {
		cout << "Base NonVirtual x = " << x << '\n';
	}
	virtual void Virtual() {
		cout << "Base Virtual x = :" << __func__ << ":" << x << '\n';
		cout << std::format("func = {:s}\n", __func__);
	}
};
class Derived : public Base {
public:
	int x;
	Derived(int a) : Base(a), x{a} {};
	void NonVirtual() {
		cout << "Derived NonVirtual ****** x = " << x << ": ";
		Base::NonVirtual();
	}
	void Virtual() {
		cout << "Derived Virtual x = " << Derived::x << ", BASE:" << Base::x << '\n';
	}
};

int main() {
	Base* bDerived = new Derived(14);

	Base bBase_o{2};
	Derived Derived_o{12};
	Base* bBase = &bBase_o;
	Derived* dDerived = &Derived_o;

	bBase->NonVirtual();  // calls base
	bBase_o.NonVirtual();
	bBase->Virtual();  // calls base
	bBase_o.Virtual();

	bDerived->NonVirtual();  // calls base !!

	bDerived->Virtual();   // call derived
	dDerived->NonVirtual();   // call derived
	dDerived->Virtual();   // call derived


	Derived_o.NonVirtual();
	Derived_o.Virtual();

	Base *b = &Derived_o;
	b->Virtual();  // Calls derived
	b->NonVirtual();  // Calls base

	Derived *d = &Derived_o;
	d->Virtual();  // Calls derived
	d->NonVirtual();  // Calls derived
}
