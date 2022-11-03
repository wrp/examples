
package main

import (
	"fmt"
	"time"
)


// Note: time.After creates a new timer object each time it is called
func fibonacci(c, q chan int) {

	timer := time.NewTimer(100 * time.Millisecond)
	defer timer.Stop()
	x, y := 1, 1
	for {
		// select blocks until some case is ready.
		// if multiple ready, run one at random
		// if default provided, execute without blocking
		select {
		case c <- x:
			x, y = y, x + y
		case <- q:
			fmt.Println("quit")

		case <- timer.C:  // expired
			fmt.Println("timeout")
			return
		}
	}
}

func main() {
	c := make(chan int)
	q := make(chan int)
	go func() {
		for i := 0; i < 10; i++ {
			fmt.Println(<-c)
		}
		// q <- 0
	}()
	fibonacci(c, q)
}
