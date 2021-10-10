package main

import (
	"fmt"
)

func maps() {
	type Vertex struct{ Lat, Long float64 }
	var m = map[string]Vertex{
		"Bell Labs": Vertex{40.68433, -74.39967},
		"Google":    Vertex{37.42202, -122.08408},
	}
	fmt.Println(m["Google"])
	for k, v := range m {
		fmt.Printf("%s -> %s\n", k, v)
	}

	p := make(map[string]int, 10)
	p["foo"] = 5
	fmt.Printf("p = %v, len = %d\n", p, len(p)) /* cap not defined for maps */

}
