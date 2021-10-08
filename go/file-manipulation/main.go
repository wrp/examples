/* Demonstrate basic file I/O */

package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
)

func check(e error) {
	if e != nil {

		/* Demonstrate write to stderr */
		fmt.Fprintf(os.Stderr, "%s: %s\n", filepath.Base(os.Args[0]), e)

		/* Reset logger to print error message without timestamp */
		/* Use log.Fatal to get the timestamp */
		l := log.New(os.Stderr, "", 0)

		/* l.Fatal(e) */
		/* Fatal will exit, but you can also do: */
		l.Println(e)
		os.Exit(1)
	}
}


func main() {
	if len(os.Args) < 2 {
		fmt.Fprintln(os.Stderr, "missing argument");
		os.Exit(1)
	}

	name := os.Args[1]

	dat, err := os.ReadFile(name)
	check(err)
	fmt.Print(string(dat))

	do_it(name)
}

func do_it(name string) {
	f, err := os.Open(name)
	check(err)

	b1 := make([]byte, 5)
	n1, err := f.Read(b1)
	check(err)
	fmt.Printf("%d bytes: %s\n", n1, string(b1[:n1]))

	o2, err := f.Seek(6, 0)
	check(err)
	b2 := make([]byte, 2)
	n2, err := f.Read(b2)
	check(err)
	fmt.Printf("%d bytes @ %d: ", n2, o2)
	fmt.Printf("%v\n", string(b2[:n2]))

	o3, err := f.Seek(6, 0)
	check(err)
	b3 := make([]byte, 2)
	n3, err := io.ReadAtLeast(f, b3, 2)
	check(err)
	fmt.Printf("%d bytes @ %d: %s\n", n3, o3, string(b3))

	_, err = f.Seek(0, 0)
	check(err)

	r4 := bufio.NewReader(f)
	b4, err := r4.Peek(5)
	check(err)
	fmt.Printf("5 bytes: %s\n", string(b4))

	f.Close()
}
