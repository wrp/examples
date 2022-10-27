

package main

import (
	"fmt"
	"strings"
)

func WordCount(s string) map[string]int {
	words := strings.Fields(s)
	count := map[string]int{}

	for _, word := range words {
		count[strings.ToLower(word)]++
	}
	return count
}

func main() {
	fmt.Println(WordCount("This is this is This is test"))
}
