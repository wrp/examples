
package runtime

import (
	"fmt"
	r "runtime"
)

func Example() {
	a := r.GOMAXPROCS(r.NumCPU())

	fmt.Println(a)
}
