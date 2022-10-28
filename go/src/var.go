
package main

import "fmt"

func main() {
	var x, y = 1, 2.3;   // Okay to mix types: x is an int, y is float64
	fmt.Printf("x is of type %T, y is of type %T\n", x, y)
}
