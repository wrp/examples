
package main

/*
#include <stdio.h>
#include <stdlib.h>
#include "print_info.h"

void print_info2(float f, int n, char *s) {
	printf("float=%.2f int=%d str=%s\n", f, n, s);
}
*/
import "C"
import "unsafe"

func main() {
	cs := C.CString("hello from cgo")
	defer C.free(unsafe.Pointer(cs))
	C.print_info(C.float(3.14), C.int(42), cs)
	C.print_info2(C.float(3.14), C.int(42), cs)
}
