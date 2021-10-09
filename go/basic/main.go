package main

import (
	"fmt"
	"runtime"
)

func main() {
	arrays()
	slices()
	maps()
	modify_slice()
	variadic()
	closure()
}

func show_func_name() {
    pc := make([]uintptr, 15)
    n := runtime.Callers(2, pc)
    frames := runtime.CallersFrames(pc[:n])
    frame, _ := frames.Next()
    fmt.Printf("**** %s\n", frame.Function)
}
