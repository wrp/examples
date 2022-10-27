
package main

import "fmt"

func main() {
	s := "abc"

Loop:  // label for break
	for n := 0; n < len(s); n += 1 {
		switch s[n]{
		case 'a': fmt.Println("read a")
		case 'b': break Loop
		case 'c': fmt.Println("read c")
		default: fmt.Printf("s[n] is %v\n", s[n])
		}
	}
	fmt.Println("Done")
}
