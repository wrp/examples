
package main

import (
	"fmt"
	"time"
	"runtime"
)

func foo() int { runtime.Gosched(); time.Sleep(2 * time.Millisecond); fmt.Printf("foo");  return 2}
func bar() int { fmt.Printf("bar"); return 1}
func baz(a, b int) int { return a + b }

func main() {

	// c := make(chan int)
	for i := 0; i < 100; i += 1 {
		a, b := foo(), bar()  // Allegedly, foo and bar are called concurrently, but this seems to always be sequential
		fmt.Println(a, b)
	}
}
