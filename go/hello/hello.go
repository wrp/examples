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
	foo();  // Call function from package main defined in foo.go
	dir, err := os.UserConfigDir()
	if err == nil {
		fmt.Println("configdir:", dir)
	}

	info, _ := debug.ReadBuildInfo()
	for _, d := range info.Deps {
		fmt.Printf("Dep: %+v\n", d)
	}


}
