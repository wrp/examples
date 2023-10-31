package main

import (
	"fmt"
	"runtime"
	"runtime/debug"
)

func main() {
	fmt.Println("hello world")
	fmt.Println("version:", runtime.Version())

	info, _ := debug.ReadBuildInfo()
	for _, d := range info.Deps {
		fmt.Printf("Dep: %+v\n", d)
	}


}
