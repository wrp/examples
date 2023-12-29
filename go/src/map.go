package main

import (
	"fmt"
	"sort"
)

func main() {
	/* map[K]V  creates a map with keys of type K and values of type V */
	m := map[int]int{ 1: 10, 2: 12, 5: 13, }
	fmt.Println(m)

	/* Print unsorted */
	fmt.Println("unsorted map with", len(m), "elements")
	for k, v := range m {
		fmt.Printf("%v -> %v\n", k, v)
	}

	s := make([]int, 0, len(m))
	for k := range m {
		s = append(s, k)
	}
	fmt.Println("presorted s = ", s)
	sort.Ints(s)
	fmt.Println("   sorted s = ", s)

	fmt.Println("sorted:")
	for _, v := range s {
		fmt.Printf("%v -> %v\n", v, m[v])
	}


	p := make(map[string]int, 10)
	p["foo"] = 5
	fmt.Printf("p = %v, len = %d\n", p, len(p)) /* cap not defined for maps */

	/* 2nd argument to make(map, hint) is a hint for the initial capacity, but
	the map will always automagically grow by the runtime.  Not finding much
	information about tuning.
	*/
}
