
package main

import "fmt"

func main() {
	slices()
	arrays()
}

func slices() {
	b := []int{1, 2, 3,}  /* An initialized slice */
	var c []int = []int{6, 7, 8, 9, 10} /* An initialized array */
	var d [][]int = [][]int{{1,2,3}, {4,5,6}}

	_, _, _ = b, c, d

	fmt.Printf("b=%v\n", b);
	fmt.Printf("b[0:]=%v\n", b[0:]);
	fmt.Printf("b[len(b)-2:]=%v\n", b[len(b)-2:]);
	fmt.Printf("len=%d cap=%d %v\n", len(c), cap(c), c)
	fmt.Printf("len=%d cap=%d %v\n", len(d), cap(d), d) /* len == cap == 2 */
}

func arrays() {
	var a [5]int           /* An array of 5 integers, initialized to 0 */
	b := [5]int{1, 2, 3,}  /* An initialized array, unset values go to 0 */
	var c [5]int = [5]int{6, 7, 8, 9, 10} /* An initialized array */
	var d [2][3] int = [2][3]int{{1,2,3}, {4,5,6}}

	_, _, _, _ = a, b, c, d
}
