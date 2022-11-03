

package main

import "fmt"


func main() {
	c := make(chan int)    // unbuffered ; send blocks
	d := make(chan int, 5) // buffered; send blocks only if full

	for i := 0; i < 2; i++ {
		d <- i  // Safe to send because channel is buffered
	}
	go func(x int, c chan int, d chan int) {
		v := <- c
		fmt.Printf("chan %d got %d from c\n", x, v)
		// close d to synchronize
		close(d)
	} (1, c, d)

	x, y := <- d, <- d
	fmt.Printf("main got %d, %d\n", x, y)

	// This won't block because above go routine is receiving
	c <- 5
	_, ok := <- d  // Wait for go routine to finish
	if ! ok {
		fmt.Println("Channel is closed")
	}

	c = make(chan int, 1);
	d = make(chan int, 1);

	c <- 2
	d <- <- c  // Receive from c and write to d
	fmt.Println("Received: ", <- d)
}
