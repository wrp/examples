
package main

import "fmt"

func intSeq() func() int {
	i := 0
	return func() int {
		i++
		return i
	}
}

func closure(){
	show_func_name()

	nextInt := intSeq()

	fmt.Printf("%d, ", nextInt())
	fmt.Printf("%d, ", nextInt())
	fmt.Printf("%d, ", nextInt())

	newInts := intSeq()
	fmt.Printf("%d\n", newInts())

	// Closures can be recursive, but this requires the closure
	// be declared with a typed `var` explicitly before it's defined.
	var fib func(n int) int
	fib = func(n int) (r int) {
		if n < 2 {
			r = n
		} else {
			p := fib(n - 2)
			r = fib(n - 1) + p
		}
		return
	}
	fmt.Println(fib(7))
}
