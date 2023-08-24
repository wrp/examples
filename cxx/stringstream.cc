

#include <iostream>
#include <sstream>
#include <string>




void foo(const std::stringstream* s, int k, int j) {
    std::cout << s->str() << ": " << k << ": " << j << '\n';
}
int main(int argc, char **argv)
{
    std::stringstream s;
    s << "bar" << 3.85 << "blah" << 2;
    // std::string ss = s.str();
    foo(&s, 1, 3);

    s.str("");
    s << "blah";
    foo(&s, 1, 3);
	return 0;
}
