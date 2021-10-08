
package main

import "fmt"

func main() {
	arrays()
	slices()
	maps()
}

func maps() {
	type Vertex struct { Lat, Long float64 }
	var m = map[string]Vertex{
		"Bell Labs": Vertex{ 40.68433, -74.39967, },
		"Google": Vertex{ 37.42202, -122.08408, },
	}
	fmt.Println("Maps");
	fmt.Println(m["Google"])

	p := make(map[string]int, 10)
	p["foo"] = 5
	fmt.Printf("p = %v, len = %d\n", p, len(p))  /* cap not defined for maps */

}

func slices() {
	var a []int
	a = make([]int, 5, 10) /* Make slice with length 5, capacity 10 */
	a[3] = 3
	a = append(a, 9)      /* Append data to a slice */
	b := []int{1, 2, 3,}  /* An initialized slice */
	var c []int = []int{6, 7, 8, 9, 10} /* An initialized array */
	var d [][]int = [][]int{{1,2,3}, {4,5,6}}

	_, _, _ = b, c, d

	fmt.Println("Slices");
	fmt.Printf("b=%v\n", b);
	fmt.Printf("b[0:]=%v\n", b[0:]);
	fmt.Printf("b[len(b)-2:]=%v\n", b[len(b)-2:]);
	fmt.Printf("a = %v: len=%d cap=%d\n", a, len(a), cap(a),)
	fmt.Printf("c = %v: len=%d cap=%d\n", c, len(c), cap(c),)
	fmt.Printf("d = %v: len=%d cap=%d\n", d, len(d), cap(d),) /* len == cap == 2 */
}

func arrays() {
	var a [5]int             /* An array of 5 integers, initialized to 0 */
	b := [5]int{1, 2, 3,}    /* An initialized array, unset values go to 0 */
	e := [...]int{1, 2, 3,}  /* An initialized array */
	var c [5]int = [5]int{6, 7, 8, 9, 10} /* An initialized array */
	var d [2][3] int = [2][3]int{{1,2,3}, {4,5,6}}
	_, _, _, _ = a, b, c, d

	fmt.Println("Arrays");
	fmt.Printf("%v: lcn=%d cap=%d\n", b, len(b), cap(b))
	fmt.Printf("%v: lcn=%d cap=%d\n", e, len(e), cap(e))

	sb := b[:]
	whatAmI := func(i interface{}) {
		switch t := i.(type) {
		default: fmt.Printf("%v is of type %T\n", i, t)
		}
	}
	whatAmI(sb)  /* is a slice */
	whatAmI(b)   /* is an array */
}
