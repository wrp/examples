

#include <iostream>
#include <vector>

using namespace std;

enum class foo {
    bar,
    qux,
};

std::ostream& operator << (std::ostream& os, const foo& obj)
{
    os << static_cast<std::underlying_type<foo>::type>(obj);
    os << (obj == foo::bar ? "(bar)" : "(qux)");
    return os;
}

int main()
{
    auto f = foo::qux;
    cout << f << '\n';
	return 0;
}
