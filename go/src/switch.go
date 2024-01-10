
package main

import "fmt"

func foo(x int) int {
	fmt.Println("foo() called with arg ", x)
	return x;
}

func main() {
	switch 4 {
	case foo(1):
		fmt.Println(1)
	case 3 - 1:
		fmt.Println("is <= 2")
	case foo(2), foo(4), 5, 6:
		fmt.Println("is = 4, 5, or 6")
		fallthrough
	case 7, foo(5):
	case 8:
		fmt.Println("is <= 8")
		fallthrough
	default:
		fmt.Println("Try again!")
	}

	switch false {
	case 1 < 2: fmt.Println("will NOT print, because expr is true")
	case 1 < 0: fmt.Println("will print, because expr is false ")
	case 1 < 0: fmt.Println("will NOT print; no fallthru")
	}

	switch { // defaults to "switch true"
	case 1 < 0: fmt.Println("will NOT print ")
	case false: fmt.Println("will NOT print ")
	case 1 < 2: fmt.Println("Does print")
	case 1 < 0: fmt.Println("will NOT print ")
	}
}
