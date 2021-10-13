package main

/*
From https://golang.org/doc/effective_go:
Maps are a convenient and powerful built-in data structure that
associate values of one type (the key) with values of another type
(the element or value). The key can be of any type for which the
equality operator is defined, such as integers, floating point and
complex numbers, strings, pointers, interfaces (as long as the
dynamic type supports equality), structs and arrays. Slices cannot
be used as map keys, because equality is not defined on them. Like
slices, maps hold references to an underlying data structure. If
you pass a map to a function that changes the contents of the map,
the changes will be visible in the caller
*/

import (
	"fmt"
)

type Vertex struct{ Lat, Long float64 }

func map_foo(x map[string]Vertex) {
	x["Google"] = Vertex{5, 6}
}

func maps() {
	var m = map[string]Vertex{
		"Bell Labs": Vertex{40.68433, -74.39967},
		"Google":    Vertex{37.42202, -122.08408},
	}
	fmt.Println(m["Google"])
	map_foo(m)
	fmt.Println(m["Google"])
	for k, v := range m {
		fmt.Printf("%s -> %s\n", k, v)
	}

	p := make(map[string]int, 10)
	p["foo"] = 5
	fmt.Printf("p = %v, len = %d\n", p, len(p)) /* cap not defined for maps */

}
