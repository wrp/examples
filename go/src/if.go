
package main

import "fmt"

func main() {
	a, b := 5, 6
	if a, b = 7, 2; a == 7 {
		fmt.Println("hello world: a = ", a, "b = ", b)
	}
}
