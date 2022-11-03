// Trivial example using sync.Mutex
package main

import (
	"fmt"
	"sync"
)


type Counter struct {
	sync.Mutex
	n int64
}

// This method is okay.
func (c *Counter) incr(d int64) (r int64) {
	c.Lock()
	c.n += d
	r = c.n
	c.Unlock()
	return
}

// The method is bad. When it is called,
// the Counter receiver value will be copied.
func (c Counter) Value() (r int64) {
	c.Lock()
	r = c.n
	c.Unlock()
	return
}

func main() {
	var c Counter
	ch := make(chan struct{})

	for i := 0; i < 100; i += 1 {
		go func() { c.incr(1); ch <- struct {}{} }()
	}
	for i := 0; i < 100; i += 1 {
		<- ch
	}
	fmt.Println( " c = ", c.n)
}
