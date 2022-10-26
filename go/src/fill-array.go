// Trivial example of concurrency.  Each goroutine initializes
// a chunk of an array

package main

import (
	"fmt"
	"sync"
)

var data [100]string

func work(n string, w chan int, wg * sync.WaitGroup) {
	fmt.Println(n)
	i := <- w;
	i *= 10
	for j := 0; j < 10; j++ {
		data[i + j] = n
	}
	wg.Done()
}

func main() {
	var wg sync.WaitGroup
	buckets := make(chan int, 10)
	for i := 0; i < 10; i++ {
		buckets <- i
	}

	for _, i := range []string{"alice", "bob", "charlie", "doug", "edwina",
			"fred", "greg", "harry", "indy", "jack"} {
		wg.Add(1)
		go work(i, buckets, &wg)
	}
	wg.Wait()

	for i, n := range data {
		if i % 10 == 0 {
			fmt.Println("")
		}
		fmt.Printf("%v: %v\t", i, n)
	}
	fmt.Println("")
}
