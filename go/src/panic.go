
package main

import (
	"fmt"
	"log"
)


func do(c <-chan int) {
	for i := range c {
		fmt.Printf("c: %v\n", i)
		if i == 5 {
			panic("I am panicing")
		}
	}
}


func safelyDo(c, d chan int) {
	defer func() {
		if err := recover(); err != nil {
			log.Println("work failed:", err)
		}
		d <- 1
	}()
	do(c)
}

func main() {
	c := make(chan int)
	d := make(chan int)
	go safelyDo(c, d)
	c <- 2
	c <- 5
	c <- 3
	close(c)
	<- d
}
