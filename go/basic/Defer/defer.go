package Defer

import (
	"fmt"
)

func Example() {
	defer fmt.Println("deferred");
	defer fmt.Println("print first");
}
