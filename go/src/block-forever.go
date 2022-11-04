// silly exmample demonstrating `select{}` idiom to block main routine

package main

import (
	"fmt"
	"time"
)



func main() {
	f := func(c chan int) {
		for a := range(c) {
			fmt.Println(a)
			time.Sleep(500 * time.Millisecond)
			c <- a + 1
		}
	}
	c := make(chan int, 1)
	c <- 1
	go f(c)
	go f(c)
	go f(c)
	select{}
}
