package main

import (
	"fmt"
	"regexp"
	"strings"
)

func main() {
	s := "This String.FOO";
	r := regexp.MustCompile(`^([\w-]+)-FOO$`).ReplaceAllString(strings.ToUpper(s), "$1-BAR")
	fmt.Println(r)
}
