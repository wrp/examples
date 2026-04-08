

struct foo {
	int x;
};

int
main()
{
	// There is no need for a typedef, and the name foo becomes
	// available as a type just by virtue of defining the struct.
	// This is a linguistic nightmare.
	foo y;
	y.x = 5;
	return 0;
}
