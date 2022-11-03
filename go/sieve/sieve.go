// Concurrent sieve of Eratosthenes

package main

import (
	"flag"
	"fmt"
)



func main() {
	var n = flag.Int("n", 10, "number of primes to find")
	flag.Parse()

	c := make(chan int)
	go func(c chan<- int) {
		for i := 2; ; i++ {
			c <- i
		}
	}(c)

	for i := 0; i < *n; i += 1 {
		prime := <-c
		fmt.Println(prime)
		ch1 := make(chan int)

		go func (in <-chan int, out chan<- int, prime int) {
			for {
				i := <-in
				if i%prime != 0 {
					out <- i
				}
			}
		}(c, ch1, prime)

		c = ch1
	}
}
