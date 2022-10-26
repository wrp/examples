
package main

import (
	"fmt"
	"reflect"
)

func make_example() {

	// make is used for slices, maps, and channels
	x := make([]int, 10, 100)  // make(T, length, capacity)
	var y []int = make([]int, 5, 50)

	fmt.Printf("x is a %v with capacity %v\n", reflect.ValueOf(x).Type(), cap(x))
	fmt.Printf("y is a %v with capacity %v\n", reflect.ValueOf(y).Type(), cap(y))
}
