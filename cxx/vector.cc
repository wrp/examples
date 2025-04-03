

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	std::vector<int> a{1,2,3};
	a.push_back(3);
	cout << "size: " << a.size() << std::endl;
	return 0;
}
