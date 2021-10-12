package main

import (
	"errors"
	"fmt"
	"os"
	"reflect"
	"sort"
)

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
		"arrays":       arrays,
		"closure":      closure,
		"interface":    interface_examples,
		"maps":         maps,
		"modify_slice": modify_slice,
		"reflect":      reflect_set,
		"scope":        scope,
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
