
package main

import "fmt"

func main() {
	var a [5]int           /* An array of 5 integers, initialized to 0 */
	b := [5]int{1, 2, 3,}  /* An initialized array, unset values go to 0 */
	var c [5]int = [5]int{6, 7, 8, 9, 10} /* An initialized array */
	var d [2][3] int = [2][3]int{{1,2,3}, {4,5,6}}

	_, _, _ = a, b, c

	fmt.Println("d:", d)

}
