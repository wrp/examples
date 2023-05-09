package array

import (
	"fmt"
)

// Passing the array as an argument is *not* idiomatic.  Doing this here
// to demonstrate (or, rather, comment on) the fact that the array is
// copied in full to pass it to the function.  Don't do this
func iterate(b [5]int) {
	// Iterate over an array
	fmt.Printf("Iterate values in b: ")
	for _, v := range b {
		fmt.Printf("%v ", v)
	}
	fmt.Println("")
}

func Example() {
	var (
		a [5]int                          /* An array of 5 integers, initialized to 0 */
		c [5]int = [5]int{6, 7, 8, 9, 10} /* An initialized array */
	)
	b := [5]int{1, 2, 3}                  /* An initialized array, unset values go to 0 */
	e := [...]int{1, 2, 3}                /* An initialized array */
	var d [2][3]int = [2][3]int{{1, 2, 3}, {4, 5, 6}}
	_, _, _, _ = a, b, c, d

	fmt.Printf("%v: lcn=%d cap=%d\n", b, len(b), cap(b))
	fmt.Printf("%v: lcn=%d cap=%d\n", e, len(e), cap(e))

	iterate(b)

	sb := b[:]
	whatAmI := func(i any) {
		switch t := i.(type) {
		default:
			fmt.Printf("%v is of type %T\n", i, t)
		case []int:
			fmt.Printf("%v is a slice of int\n", i)
		}
	}
	whatAmI(sb) /* is a slice */
	whatAmI(b)  /* is an array */
}
