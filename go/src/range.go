// Demonstrate range on a channel

package main

import "fmt"

func main() {
	c := make(chan int, 2)
	c <- 3
	go func (c chan int) {
		for i := 0; i < 10; i++ {
			c <- i
		}
		close(c)
	}(c)

	for i := range c {
		fmt.Printf("received: %d\n", i)
	}
	_, ok := <- c
	fmt.Printf("ok is %v\n", ok)
}
