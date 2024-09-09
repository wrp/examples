
// Demonstrate how append is unsafe.  It's similar to realloc,
// so the slice may refer to a different underlying array.
// In short, you cannot ever write a = append(a, ...)
// for the same reason you cannot write (in C) v = realloc(v, ...)
// If you do that, you have no way of knowing if the slice
// refers to the same underlying array.

// Derived from https://go.dev/play/p/UZSlBMUlc_E

package main

import (
	"fmt"
)

func tweak(a []int, b []int) {
	fmt.Printf("before: cap(a) = %v", cap(a))
	fmt.Printf("\ta[%v] = %v\n", len(a) - 1, a[len(a)-1])

	a = append(a, 1)
	b = append(b, 2)   // If a did not grow, this changes a
	b[0] = 5   // Similarly, this sometimes changes the underlying array
	i := len(a) - 1

	fmt.Printf("after: cap(a) = %v", cap(a))
	fmt.Printf("\ta[%v] = %v\n", i, a[i])
	fmt.Println(a[i], b[i])
}

func main() {
	// make a slice with room to grow and one without
	for length := 5; length < 11; length += 5 {
		a := make([]int, length, 10)
		b := a[:]
		tweak(a, b)
		// If a had room to append, then a[0] will be modified
		// by tweak.  Otherwise not.
		fmt.Printf("in main, a[%v] = %v\n", 0, a[0])
	}
}
