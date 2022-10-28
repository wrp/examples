
package main

import (
	"fmt"
	"golang.org/x/exp/constraints"
)

func main() {
	x := GMin[int](2, 3)
	fmt.Println("generic min is:", x)
}



// Generic min
func GMin[T constraints.Ordered](x, y T) T {
	if x < y {
		return x
	}
	return y
}
