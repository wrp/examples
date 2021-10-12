package main

import (
	"fmt"
	"reflect"
)

func structs() {
	type T struct {
		A int
		B string
		C float64
	}
	t := T{23, "skidoo", 3.5}
	s := reflect.ValueOf(&t).Elem()
	typeOfT := s.Type()
	// Iterate over the members of a struct, printing their names/values
	for i := 0; i < s.NumField(); i++ {
		f := s.Field(i)
		fmt.Printf("%d: %s %s = %v\n", i,
			typeOfT.Field(i).Name, f.Type(), f.Interface())
	}
}
