
package main

import "fmt"

func main() {
	fmt.Println("hello world")
	fmt.Println("hello", "world")      // Println does insert space

	// Print inserts a space between arguments only if neither is a string
	fmt.Print("hello", "world", "\n")  // Print does *not* insert space
	fmt.Print("hello", 6, 7, "world", "\n")  // Does insert space between 6 and 7


}
