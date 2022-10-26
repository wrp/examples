package array

import (
	"fmt"
)

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

	sb := b[:]
	whatAmI := func(i interface{}) {
		switch t := i.(type) {
		default:
			fmt.Printf("%v is of type %T\n", i, t)
		}
	}
	whatAmI(sb) /* is a slice */
	whatAmI(b)  /* is an array */
}
