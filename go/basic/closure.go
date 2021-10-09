
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
}
