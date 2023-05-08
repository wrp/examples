
package main

import (
	"bufio"
	"fmt"
	"os"
)

func is_palindrome(s string) bool {
	k := len(s) - 1
	for i := 0; i < k; i += 1 {
		if( s[i] != s[k] ){
			return false
		}
		k -= 1
	}
	return true;
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		s := scanner.Text();
		fmt.Printf("%s: %v\n", s, is_palindrome(s));
	}
}
