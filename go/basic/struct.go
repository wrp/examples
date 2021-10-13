package main

import (
	"fmt"
	"reflect"
)

type T struct {
	A int
	B string
	C float64
}
func (t T) String() string {
	return fmt.Sprintf("A=%v, B=%v, C=%v", t.A, t.B, t.C)
}

func structs() {
	t := T{23, "skidoo", 3.5}
	s := reflect.ValueOf(&t).Elem()
	typeOfT := s.Type()
	// Use %+v to print elements of the struct  (this seems identical to %v)
	fmt.Printf("%%v vs: %%+v: %v --> %+v\n", t, t)

	// Iterate over the members of a struct, printing their names/values
	for i := 0; i < s.NumField(); i++ {
		f := s.Field(i)
		fmt.Printf("%d: %s %s = %v\n", i,
			typeOfT.Field(i).Name, f.Type(), f.Interface())
	}

	// new(T) is exactly the same as &T{}
	v := new(T)
	w := &T{}
	x := &T{A:5, B:"foo", C: 3}

	fmt.Println(v)
	fmt.Println(w)
	fmt.Println(x)
}
