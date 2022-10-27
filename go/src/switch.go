
package main

import "fmt"

func main() {
	n := 4
	switch n {
	case 0:
		fmt.Println("is zero")
		fallthrough
	case 1:
		fmt.Println("is <= 1")
		fallthrough
	case 3 - 1:
		fmt.Println("is <= 2")
	case 4, 5, 6:
		fmt.Println("is = 4, 5, or 6")
		fallthrough
	case 7:
		fallthrough
	case 8:
		fmt.Println("is <= 8")
		fallthrough
	default:
		fmt.Println("Try again!")
	}

	switch false {
	case 1 < 2: fmt.Println("will NOT print")
	case 1 < -5: fmt.Println("will print ")
	}

	switch { // defaults to "switch true"
	case 1 < 2: fmt.Println("Does print")
	case 1 < -5: fmt.Println("will print ")
	}
}
