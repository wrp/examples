
package main

import (
	"fmt"
	"golang.org/x/exp/constraints"
)

func main() {
	fmt.Println("generic min is:", GMin[int](2, 3)) // Make call with explicit type
	fmt.Println("generic min is:", GMin(2, 3)) // Make call with implicit type

	fmt.Println("generic min is:", GMin[float32](2.0, 3.0)) // Make call with explicit type
	fmt.Println("generic min is:", GMin(2.0, 3.0)) // Make call with explicit type
}



// Generic min
func GMin[T constraints.Ordered](x, y T) T {
	if x < y {
		return x
	}
	return y
}
