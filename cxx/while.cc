

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	if(int i = 0; i < 5) {
		i += 1;
		switch(i){
		case 0:
			cout << i << endl;
			break;
		case 1:
			cout << i << ": fallthru" <<endl;
		case 2:
			cout << i << endl;
		case 3:
		case 4:
		case 5:
			cout << i << endl;
		}
		cout << "end of switch" << endl;
	}
	return 0;
}
