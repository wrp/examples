// A simple implementation of cat
// keywords: fgetc fread stdin stdout getc
package main

import (
	"fmt"
	"path/filepath"
	"io"
	"os"
)

func cat_file(f *os.File) {
	n := 1024
	b := make([]byte, n)
	for {
		n, err := f.Read(b)
		if (err == io.EOF) {
			break;
		}
		check(err)
		_, err = os.Stdout.Write(b[0:n])
		check(err)
	}
	return
}

func cat_name(path string){
	var f *os.File
	if path == "-" {
		f = os.Stdin
	} else {
		f, err := os.Open(path)
		check(err)
		defer func() {
			err = f.Close()
			check(err)
		}()
	}
	cat_file(f)
}

func check(e error) {
	if e != nil {
		fmt.Fprintf(os.Stderr, "%s: %s\n", filepath.Base(os.Args[0]), e)
		os.Exit(1)
	}
}

func main() {
	if len(os.Args) == 1 {
		os.Args = append(os.Args, "-")
	}
	for _, name := range os.Args[1:] {
		cat_name(name)
	}
}
