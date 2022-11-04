
package main

import (
	"fmt"
)

func foo(c chan<- struct{}) int { c <- struct{}{}; fmt.Printf("foo");  return 2}
func bar(c <-chan struct{}) int { fmt.Printf("bar"); <- c ;  return 1}
func baz(a, b int) int { return a + b }

func main() {

	c := make(chan struct{})

	// Allegedly, foo and bar are called concurrently, but this seems to always be sequential
	// Indeed, the language spec guarantees a deadlock here, since foo() must return before
	// bar() is executed.  The rumor that the calls are concurrent is simply false.
	a, b := foo(c), bar(c)
	fmt.Println(a, b)
}
