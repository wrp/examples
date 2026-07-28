
package main

import "fmt"

func main() {
	var p func(...any) (int, error)
	p = fmt.Println
	p("hello world")
	p("hello", "world")      // Println does insert space

	// Print inserts a space between arguments only if neither is a string
	p = fmt.Print
	p("hello", "world", "\n")  // Print does *not* insert space
	p("hello", 6, 7, "world", "\n")  // Does insert space between 6 and 7


}
