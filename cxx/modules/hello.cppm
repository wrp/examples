
export module hello_utils; // Define the module name

// Without export, this is internal to the module
int
private_multiply(int x, int y) {
	return x * y;
}

// Public interface
export int
multiply_add(int a, int b) {
	return private_multiply(a, a + b);
}
