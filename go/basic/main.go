package main

import (
	"fmt"
	"errors"
	"reflect"
)

func call(m map[string]interface{}, name string, params ... interface{}) (result []reflect.Value, err error) {
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
		"arrays": arrays,
		"slices": slices,
		"maps": maps,
		"modify_slice": modify_slice,
		"variadic": variadic,
		"closure": closure,
	}
	call(f, "arrays")
	call(f, "slices")
	call(f, "maps")
	call(f, "modify_slice")
	call(f, "variadic")
	call(f, "closure")
}
