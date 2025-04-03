// Silly toy to print control sequences to reverse video text
package main

import (
	"fmt"
)

func main() {
	// enable revers video
	fmt.Printf("\x1b\x5b\x37\x6d")
	fmt.Printf("hello")
	fmt.Printf("\x1b\x5b\x6d\x0f")
	fmt.Println(" world")
}
