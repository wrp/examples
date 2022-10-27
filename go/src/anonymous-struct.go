
package main

import "fmt"

func main() {

	diana := struct {
		firstName, lastName string
		age                 int
	}{
		firstName: "Diana",
		lastName:  "Zimmermann",
		age:       30,
	}
	fmt.Printf("diana = %+v\n", diana)
// We can define a struct type without any field names. We call them anonymous fields.
// All we have to do is mention the field types and Go will use type as a field name.
// Example of a struct with anonymous fields:

	type Book struct {
		string
		float64
		bool
	}
	myBook := Book{"1984 by George Orwell", 10.2, false}
	fmt.Printf("my book = %v\n", myBook)
	fmt.Printf("my book title = %v\n", myBook.string)
}
