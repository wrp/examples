package main

import (
	"fmt"
	"reflect"
)

func reflect_set() {

	var x float64 = 3.4
	p := reflect.ValueOf(&x) // Note: take the address of x.
	fmt.Println("type of p:", p.Type())
	fmt.Println("settability of p:", p.CanSet())

	v := p.Elem()
	fmt.Println("settability of v:", v.CanSet())

	fmt.Printf(" unmodified v = %v, x = %v\n", v.Interface(), x)
	v.SetFloat(7.1)
	fmt.Printf(" unmodified v = %v, x = %v\n", v.Interface(), x)
}
