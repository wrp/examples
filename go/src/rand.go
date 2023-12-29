
package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {
	rand.Seed(time.Now().UnixNano())
	fmt.Println("A random int in [0, 5): ", rand.Int31n(5))

	// Int31n(n) returns a random 31-bit integer as an int32 in
	// the half-open interval [0,n)
	// AFAICT, the language mandates that integers be stored in
	// 2s complement, or at least the documentation certainly
	// seems to make that assumption
}
