
package main

import "testing"

func TestFoo(t *testing.T) {
	k := foo()
	if k != 1 {
		t.Errorf("Foo did not return 1 %v", k)
	}
}
