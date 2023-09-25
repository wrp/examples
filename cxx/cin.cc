
#include <iostream>
#include <vector>

using namespace std;

/* If there is a conversion error reading, then subsequent
 * attempts to read will fail until cin.clear() is called.
 * eg: int x; cin >> x; will fail if the stream cannot be
 * converted.
 *
 * This is basically scanf, but with uglier notation.  See
 * std::format for output.
 */
int main()
{
	int i;
	char c;
	if( cin.good() ){
		cin >> i >> c;
		cout << "i is " << i << endl;
		cout << "c is " << c << endl;
	}
	cout << "cin.bad " << cin.bad() << endl;
	cout << "cin.good" << cin.good() << endl;
	cout << "cin.eof " << cin.eof() << endl;
	cout << "cin.fail " << cin.eof() << endl;
	if( cin.rdstate() ){
		cout << "cin in state: " << cin.rdstate() << endl;
		cin.clear();
	}
	cin >> i >> c;
	cout << "i is " << i << endl;
	cout << "c is " << c << endl;
}
