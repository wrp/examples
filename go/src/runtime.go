// Demonstrate runtime.Gosched() to yield

package main

import (
	"fmt"
	"runtime"
)

func main() {
	a := 1
	c := make(chan struct{})
	for i := 1; i < 5; i += 1 {
		go func(i int, c chan<- struct{}) {
			a += 1
			fmt.Println("In ", i, a)
			runtime.Gosched()
			fmt.Println("after Gosched: ", i, a);
			c <- struct{}{}
		}(i, c)
	}

	for i := 1; i < 5; i += 1 {
		<- c
	}

	fmt.Println("In main after Gosched:\n", a);
}
