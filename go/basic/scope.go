
package main

import (
	"fmt"
	"go/importer"
	"go/types"
	"os"
)

func scope() {
	pkg, err := importer.Default().Import("time")
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err.Error())
		return
	}

	scope := pkg.Scope()
	for _, name := range scope.Names() {
		fmt.Printf("name = %s:\t", name);
		obj := scope.Lookup(name)
		if tn, ok := obj.Type().(*types.Named); ok {
			fmt.Printf("method count: %#v", tn.NumMethods())
		}
		fmt.Printf("\n")
	}
}
