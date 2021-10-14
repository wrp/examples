package main

import (
	"example/basic/array"
	"example/basic/closure"
	"example/basic/iota"
	"example/basic/lock"
	"example/basic/map"
	"example/basic/print"
	wrun "example/basic/runtime"
	"example/basic/scope"
	"errors"
	"fmt"
	"os"
	"reflect"
	"runtime"
	"sort"
)


func init() {
	s := ""
	if _, fileName, fileLine, ok := runtime.Caller(0); ok {
		s = fmt.Sprintf("%s:%d", fileName, fileLine)
	}
	fmt.Println("init in ", s, "called")
}


func call(m map[string]interface{}, name string, params ...interface{}) (result []reflect.Value, err error) {
	fmt.Printf("**** %s ****\n", name)
	f := reflect.ValueOf(m[name])
	if len(params) != f.Type().NumIn() {
		err = errors.New("The number of params is not adapted.")
		return
	}
	in := make([]reflect.Value, len(params))
	for k, param := range params {
		in[k] = reflect.ValueOf(param)
	}
	result = f.Call(in)
	return
}

func main() {
	f := map[string]interface{}{
		"arrays":       array.Example,
		"closure":      closure.Example,
		"defer":        defer_example,
		"interface":    interface_examples,
		"iota":         iota.Example,
		"lock":	        lock.Example,
		"make":         make_example,
		"maps":         maps.Example,
		"modify_slice": modify_slice,
		"print":        print.Example,
		"reflect":      reflect_set,
		"runtime":      wrun.Example,
		"scope":        scope.Example,
		"slices":       slices,
		"struct":       structs,
		"variadic":     variadic,
	}
	args := os.Args[1:]
	if len(args) == 0 {
		args = make([]string, len(f))
		i := 0
		for k := range f {
			args[i] = k
			i += 1
		}
		sort.Strings(args)
	}
	for i := range args {
		if _, ok := f[args[i]]; ok {
			call(f, args[i])
		} else {
			fmt.Fprintln(os.Stderr, args[i], ": unkown")
		}
	}
}
