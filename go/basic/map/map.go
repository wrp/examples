package maps

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

Concurrent read/write is undefined behavior.
*/

import (
	"fmt"
)

type vertex struct{ Lat, Long float64 }

func map_incr(x map[int]int) {
	for k := range x {
		x[k] += 1
	}
}

func Example() {
	/* map[K]V  creates a map with keys of type K and values of type V */
	var m = map[string]vertex{
		"Bell Labs": vertex{40.68433, -74.39967},
		"Google":    vertex{37.42202, -122.08408},
	}
	fmt.Printf("map = %v\n", m)

	b := []string{"Google", "Bell Labs"} // Missing index returns 0
	delete(m, "Bell Labs")  // Remove entry from a map

	fmt.Println("after deletion, map is", m)

	for _, v := range b {
		if loc, ok := m[v]; ok {
			fmt.Println(v, "is at", loc)
		} else {
			fmt.Println(v, "is not in the map")
		}
	}
	k := map[int]int{
		1: 10,
		2: 12,
		5: 13,
	}
	map_incr(k)
	fmt.Println(k)
	for k, v := range m {
		fmt.Printf("%s -> %v\n", k, v)
	}

	p := make(map[string]int, 10)
	p["foo"] = 5
	fmt.Printf("p = %v, len = %d\n", p, len(p)) /* cap not defined for maps */
}
