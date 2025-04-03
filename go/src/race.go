// Run with "go run -race race.go" to see warning

// $ go run -race race.go
// hmm
// ==================
// WARNING: DATA RACE
// Write at 0x00c000118020 by goroutine 7:
//   main.main.func1()
//       .../race.go:34 +0x6f
//
// Previous write at 0x00c000118020 by main goroutine:
//   main.main()
//       .../race.go:37 +0x10f
//
// Goroutine 7 (running) created at:
//   main.main()
//       .../race.go:33 +0xc4
// ==================
// Found 1 data race(s)
// exit status 66


package main

import (
	"fmt"
	"errors"
)

func main() {
	var err error

	go func() {
		err = errors.New("foo")
	}()

	err = errors.New("main")
	if err != nil {
		fmt.Println("hmm")
	}
}
