

/* Count words in the input stream */
#include <unordered_map>
#include <iostream>

using namespace std;

int
get(unordered_map<std::string, int> m, std::string key)
{
	auto i = m.find(key);
	if( i == m.end() )
		return 0;
	return i->second;
}

int
main(int argc, char **argv)
{
	unordered_map<std::string, int> m;
	std::string s;

	while( cin >> s ){
		m[s] = get(m, s) + 1;
	}
	for (auto& it: m) {
		cout << it.first << ": " << it.second << '\n';
	}
}
