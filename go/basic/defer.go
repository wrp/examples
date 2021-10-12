

package main

import (
	"fmt"
)

func defer_example() {
	defer fmt.Printf("\n")
	for i := 0; i < 5; i++ {
		defer fmt.Printf("%d ", i)
	}
}
