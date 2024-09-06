package main

import (
	"fmt"
	"os"
	"runtime"
	"runtime/debug"
)

func main() {
	fmt.Println("hello world")
	fmt.Println("version:", runtime.Version())
	dir, err := os.UserConfigDir()
	if err == nil {
		fmt.Println("configdir:", dir)
	}

	info, _ := debug.ReadBuildInfo()
	for _, d := range info.Deps {
		fmt.Printf("Dep: %+v\n", d)
	}


}
