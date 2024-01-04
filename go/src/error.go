
package main

import (
	"errors"
	"fmt"
	"os"
	"math"
	"strconv"
)


func add(a, b float64) (c float64, err *MyError) {
	// Return the sum of a and b if both are even.  Else throw an error
	// (This is a silly exercise to demonstrate errors)
	err = &MyError{}

	if (int64(math.Floor(a)) % 2) != 0 || (int64(math.Floor(b)) % 2) != 0 {
		err.error = fmt.Errorf("Oops, one operand was odd");
		err.int = 17
		return
	}
	c = a + b
	return

}

type MyError struct {
	int
	error
}

func (e *MyError) Error() string {
	return fmt.Sprintf("%s: %d", e.error, e.int)
}

func main() {

	var a, b float64;
//	err  := fmt.Errorf("stuv")
	var err *MyError = &MyError{}

	err.error = errors.New("")

	fmt.Printf("error is of type: %T\n", err)

	if len(os.Args) > 1 {  // keyword: argv
		var e error
		a, e = strconv.ParseFloat(os.Args[1], 64)
		err = e.(*MyError)
	} else {
		a = float64(1)
	}
	if len(os.Args) > 2 {
		var e error
		b, e = strconv.ParseFloat(os.Args[2], 64)
		err = e.(*MyError)
	} else {
		b = float64(1)
	}

	c, err := add(a, b);

	if err.error != nil {
		fmt.Fprintf(os.Stderr, "%s\n", err)
	} else {
		fmt.Printf("%v + %v = %v\n", a, b, c);
	}
}
