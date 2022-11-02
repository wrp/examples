
package main

import (
	"errors"
	"fmt"
	"os"
	"math"
	"strconv"
)


func add(a, b float64) (c float64, err error) {
	// Return the sum of a and b if both are even.  Else throw an error
	// (This is a silly exercise to demonstrate errors)

	if (int64(math.Floor(a)) % 2) != 0 || (int64(math.Floor(b)) % 2) != 0 {
		err = fmt.Errorf("Oops, one operand was odd");
		return
	}
	c = a + b
	return

}

func main() {

	var a, b float64;
//	err  := fmt.Errorf("stuv")
	err  := errors.New("")

	fmt.Printf("error is of type: %T\n", err)

	if len(os.Args) > 1 {
		a, err = strconv.ParseFloat(os.Args[1], 64)
	} else {
		a = float64(1)
	}
	if len(os.Args) > 2 {
		b, err = strconv.ParseFloat(os.Args[2], 64)
	} else {
		b = float64(1)
	}

	c, err := add(a, b);


	if err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: %v\n", err)
	} else {
		fmt.Printf("%v + %v = %v\n", a, b, c);
	}
}
