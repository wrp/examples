
#include <iostream>
#include <streambuf>
#include <locale>
#include <cstdio>

class outbuf : public std::streambuf
{
	protected:

	/* central output function - print characters in uppercase mode */
	virtual int_type overflow (int_type c) {
		if( c != EOF ){
			// convert lowercase to uppercase
			c = std::toupper(static_cast<char>(c),getloc());

			// and write the character to the standard output
			if( putchar(c) == EOF ){
				return EOF;
			}
		}
		return c;
	}
};

int
main()
{
	outbuf ob;
	std::ostream out(&ob);
	out << "31 hexadecimal: " << std::hex << 31 << std::endl;
	return 0;
}
