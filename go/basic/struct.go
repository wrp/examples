package main

import (
	"fmt"
	"reflect"
)

type P struct {
	x, y int
}

type T struct {
	A int
	B string
	C float64
	P
}
func (t T) String() string {
	return fmt.Sprintf("A=%v, B=%v, C=%v", t.A, t.B, t.C)
}

func structs() {
	t := T{23, "skidoo", 3.5, P{0 , 0}}
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
	x := &T{5, "foo", 3, P{x: 0, y:5} }

	fmt.Println(v)
	fmt.Println(w)
	fmt.Println(x)

	fmt.Printf("Using %%+: %+v\n", x)
	fmt.Printf("Using %%+: %+v\n", x.y)
}
