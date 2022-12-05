
package Error
import (
	"fmt"
)


// https://go.dev/blog/go1.13-errors

func Example() {
	err := foo(5)
	if err != nil {
		fmt.Printf("Example: %s\n", err)
	}
}

type FooError struct {
	Err error;
	Val int;
}

type BarError struct {
	Err error;
	Val int;
}

// Errors should implement an Error method
func (e *FooError) Error() string { return fmt.Sprintf("%d: %w", e.Val, e.Err.Error()) }
func (e *BarError) Error() string { return fmt.Sprintf("%d: %w", e.Val, e.Err.Error()) }

func foo(v int) error {
	err := bar(v + 1)
	return fmt.Errorf("foo: %w", err)
}

func bar(v int) error {
	return fmt.Errorf("bar: %d", v)
}
