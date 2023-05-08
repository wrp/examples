
package main

import (
	"testing"
)


func Test_is_palindrome(t *testing.T) {
	samples := [...]string{
		"amanaplanacanalpanama",
		"",
		"foof",
		"fof",
	}

	for _, s := range samples {
		if ! is_palindrome(s) {
			t.Fatalf(`%s did not match`, s)
		}
	}
}
