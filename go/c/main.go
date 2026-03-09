
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

	info := C.struct_Info{
		n: C.int(42),
		f: C.float(3.14),
		s: cs,
	}
	C.print_info(&info)
	C.print_info2(C.float(3.14), C.int(42), cs)
}
