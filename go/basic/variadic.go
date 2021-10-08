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
	fmt.Println("sum is", sum(5.2, 2.9, -4e-5))
}
