

/* Count words in the input stream */
#include <unordered_map>
#include <iostream>

using namespace std;


int
main(int argc, char **argv)
{
	unordered_map<std::string, int> m;
	std::string s;

	/*
	 * m.at() throws only std::out_of_range.
	 * This is equivalent to "m[s] += 1", which
	 * default-inserts 0 for missing keys.
	 */
	while (cin >> s) {
		try {
			m.at(s) += 1;
		} catch (const std::out_of_range& e) {
			m[s] = 1;
		}
	}
	for (auto& it: m) {
		cout << it.first << ": " << it.second << '\n';
	}
}
