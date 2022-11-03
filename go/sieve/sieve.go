// Concurrent sieve of Eratosthenes


package main

import (
	"flag"
	"fmt"
)

func main() {
	var n = flag.Int("n", 100, "max value to check")
	flag.Parse()

	w := make(chan chan int, 1);
	d := make(chan int);
	w <- d

	for i := 2; i < *n; i += 1 {
		c := make(chan int);

		go func(m int, in chan int, out chan int ) {
			for k := range(in) {
				fmt.Printf("%d -> %d\n", m, k)
				if k % m != 0 {
					out <- k
				}
			}
			close(out)
		}(i, <- w, c)

		w <- c
	}

	close(w)
	for i := 0; i < *n; i += 1 {
		d <- i
	}
	close(d)


	for p := range( <-w ) {
		fmt.Println(p)
	}
}
