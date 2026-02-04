

#include <iostream>
#include <vector>

using namespace std;

class outer {
public:
	enum class foo {
		bar = 5,
		qux,
	};
};

	std::ostream& operator << (std::ostream& os, const outer::foo& obj)
	{
		os << static_cast<std::underlying_type<outer::foo>::type>(obj);
		string v{};
		switch(obj){
		case outer::foo::bar: v = "(bar)"; break;
		case outer::foo::qux: v = "(qux)"; break;
		default: v = "(??\?)"; break;
		}
		os << v;
		return os;
	};

int main()
{
	auto f = outer::foo::qux;
	cout << f << '\n';
	for( int i = 0; i < 10; i += 1 ) {
		auto g = static_cast<outer::foo>(i);
		cout << i << ": " << g << '\n';
	}
	unsigned k = static_cast<unsigned>(f);
	cout << "Cast " << f << " to unsigned: " << k << endl;

	return 0;
}
