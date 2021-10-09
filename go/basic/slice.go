
package main
import (
	"fmt"
	"bytes"
)

func slices() {
	var a []int
	a = make([]int, 5, 10) /* Make slice with length 5, capacity 10 */
	a[3] = 3
	a = append(a, 9)                    /* Append data to a slice */
	b := []int{1, 2, 3}                 /* An initialized slice */
	var c []int = []int{6, 7, 8, 9, 10} /* An initialized array */
	var d [][]int = [][]int{{1, 2, 3}, {4, 5, 6}}

	/* Iterate over a slice */
	sum := 0
	for i := range a {
		sum += a[i]
	}
	fmt.Println("Sum of elments in a: ", sum)

	fmt.Printf("b=%v\n", b)
	fmt.Printf("b[0:]=%v\n", b[0:])
	fmt.Printf("b[len(b)-2:]=%v\n", b[len(b)-2:])
	fmt.Printf("a = %v: len=%d cap=%d\n", a, len(a), cap(a))
	fmt.Printf("c = %v: len=%d cap=%d\n", c, len(c), cap(c))
	fmt.Printf("d = %v: len=%d cap=%d\n", d, len(d), cap(d)) /* len == cap == 2 */
}


type path []byte

func (p *path) dirname() {
	i := bytes.LastIndex(*p, []byte("/"))
	if i >= 0 {
		*p = (*p)[0:i]
	}
}

func modify_slice() {
    pathName := path("/usr/bin/tso")
    pathName.dirname()
    fmt.Printf("%s\n", pathName)
}
