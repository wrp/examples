// Implement a simple LRU cache
package main

import (
	"fmt"
	"os"
	"strconv"
	"math/rand"
)

type cache_entry struct {
	value int
	next *cache_entry
	prev *cache_entry
}

type cache struct {
	data map[int] *cache_entry
	oldest, newest *cache_entry;
	cap int;
}

// Take ce out of the list
func (ce *cache_entry) purge() {
	if (ce.prev != nil) {
		ce.prev.next = ce.next
	}
	if (ce.next != nil) {
		ce.next.prev = ce.prev
	}
}

func new_cache(size int) *cache {
	var c cache
	c.data = make(map[int] *cache_entry, size)
	c.cap = size
	c.oldest = nil
	c.newest = nil
	return &c
}

func (c *cache) pop() (i int, e error) {
	if c.newest == nil {
		e = fmt.Errorf("empty cache")
	}
	i = c.newest.value
	c.newest = c.newest.next
	delete(c.data, i)
	return
}

func (c *cache) rm_oldest() {
	if c.oldest == nil {
		panic("ack")
	}
	i := c.oldest.value
	c.oldest = c.oldest.prev
	c.oldest.next = nil
	delete(c.data, i)
	return
}

func (c *cache) push(v int) {
	old, ok := c.data[v]
	if ok {
		if old == c.newest {
			return
		}
		old.purge()
	} else if len(c.data) >= c.cap {
		c.rm_oldest()
	}

	ce := &cache_entry {
		value: v,
		next: c.newest,
		prev: nil,
	}
	if c.oldest == nil {
		c.oldest = ce
	}
	if c.newest != nil {
		c.newest.prev = ce
	}
	c.newest = ce
	c.data[v] = ce
}



func (c *cache) fill(count int) {
	for i := 0; i < count; i++ {
		c.push(int(rand.Int31n(65536)))
	}
}


func main() {
	c := new_cache(3)
	c.fill(5)
	for _, a := range os.Args[1:] {
		v, _ := strconv.Atoi(a)
		c.push(v)
	}
	fmt.Println("Full contents of cache:")
	for k := range c.data { fmt.Println(k); }
	fmt.Println("*** in order of recent use:***")
	for i := 0; i < 3; i += 1 {
		v, e := c.pop()
		if e == nil {
			fmt.Println(v)
		}
	}
}
