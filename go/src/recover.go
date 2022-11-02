// Demonstrate using recover to get default args

package main

import (
	"fmt"
	"time"
	"os"
)

type args struct {
	duration time.Duration
}

func parseArgs() (a args) {
	defer func() {
		if err := recover(); err != nil {
			a.duration, _ = time.ParseDuration("5m")
		}
	}()
	a.duration, _ = time.ParseDuration(os.Args[1])
	return
}

func main() {
	a := parseArgs()

	fmt.Printf("%v\n", a)
}

