
package main

import (
	"fmt"
	"math/rand/v2"
)

func main() {
	// As of Go 1.20+, the global random generator is automatically seeded
	// rand/v2 (Go 1.22+) provides a better API without needing Seed()
	fmt.Println("A random int in [0, 5): ", rand.IntN(5))

	// IntN(n) returns a random integer in the half-open interval [0,n)
	// Go relies on the underlying architecture's integer representation
}
