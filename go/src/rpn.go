// A simple reverse-polish notation calculator
// keywords: rpn stack postfix calculator
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	var stack []float64

	push := func(v float64) {
		stack = append(stack, v)
	}

	pop := func() (float64, bool) {
		if len(stack) == 0 {
			return 0, false
		}
		v := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		return v, true
	}

	pop2 := func() (a, b float64, ok bool) {
		b, ok = pop()
		if !ok {
			return
		}
		a, ok = pop()
		return
	}

	scanner := bufio.NewScanner(os.Stdin)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		token := scanner.Text()

		if n, err := strconv.ParseFloat(token, 64); err == nil {
			push(n)
			continue
		}

		switch token {
		case "+":
			a, b, ok := pop2()
			if !ok {
				fmt.Fprintln(os.Stderr, "stack underflow")
				continue
			}
			push(a + b)
		case "-":
			a, b, ok := pop2()
			if !ok {
				fmt.Fprintln(os.Stderr, "stack underflow")
				continue
			}
			push(a - b)
		case "*", "x":
			a, b, ok := pop2()
			if !ok {
				fmt.Fprintln(os.Stderr, "stack underflow")
				continue
			}
			push(a * b)
		case "/":
			a, b, ok := pop2()
			if !ok {
				fmt.Fprintln(os.Stderr, "stack underflow")
				continue
			}
			if b == 0 {
				fmt.Fprintln(os.Stderr, "division by zero")
				continue
			}
			push(a / b)
		case "p":
			if len(stack) == 0 {
				fmt.Fprintln(os.Stderr, "stack empty")
			} else {
				fmt.Println(stack[len(stack)-1])
			}
		case "f":
			for i := len(stack) - 1; i >= 0; i-- {
				fmt.Println(stack[i])
			}
		case "c":
			stack = stack[:0]
		case "d":
			if len(stack) == 0 {
				fmt.Fprintln(os.Stderr, "stack empty")
			} else {
				push(stack[len(stack)-1])
			}
		case "r":
			a, b, ok := pop2()
			if !ok {
				fmt.Fprintln(os.Stderr, "stack underflow")
				continue
			}
			push(b)
			push(a)
		case "q":
			return
		default:
			fmt.Fprintf(os.Stderr, "unknown token: %s\n", token)
		}

		if strings.ContainsAny(token, "+-*/x") && len(stack) > 0 {
			fmt.Println(stack[len(stack)-1])
		}
	}
}
