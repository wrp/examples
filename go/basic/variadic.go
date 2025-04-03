package main

import (
	"fmt"
)

func sum(floats ...float32) (ret float32) {
	for _, v := range floats {
		ret += v
	}
	return
}

func variadic() {
	fmt.Println("hardcoded sum is", sum(5.2, 2.9, -4e-5))

	a := []float32{2.3, 1.0, 5}

	/* Explode the slice to call the variadic function */
	s := sum(a...)
	fmt.Println("sum of %v sum is %v", a, s)
}
