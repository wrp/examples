
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
	fmt.Println("Closures")

	nextInt := intSeq()

	fmt.Printf("%d, ", nextInt())
	fmt.Printf("%d, ", nextInt())
	fmt.Printf("%d, ", nextInt())

	newInts := intSeq()
	fmt.Printf("%d\n", newInts())

	fmt.Println(fact(3))

	// Closures can also be recursive, but this requires the
	// closure to be declared with a typed `var` explicitly
	// before it's defined.
	var fib func(n int) int

	fib = func(n int) int {
		if n < 2 {
			return n
		}
		return fib(n-1) + fib(n-2)

		// Since `fib` was previously declared in `main`, Go
		// knows which function to call with `fib` here.
	}

	fmt.Println(fib(3))
}


// Go supports
// <a href="http://en.wikipedia.org/wiki/Recursion_(computer_science)"><em>recursive functions</em></a>.
// Here's a classic example.


// This `fact` function calls itself until it reaches the
// base case of `fact(0)`.
func fact(n int) int {
	if n == 0 {
		return 1
	}
	return n * fact(n-1)
}
