package main


import (
	"fmt"
)

// order:
//  1. Package-level variable initialization (the var blocks)
//  2. init() functions
//  3. main() (for package main)

var (
	a, err = fmt.Println("1. this happens first");
)

func init() {
	fmt.Println("2. then this");
	defer func() {
		fmt.Println("3. This is 3rd");
	} ()

}


func main() {
	fmt.Println("4. this is last");
	fmt.Printf("a is of type %T\n", a);
	fmt.Printf("err is of type %T\n", err);
}
