#Spec: {
	kind: string

	name: {
		first:   != "" // must be specified and non-empty
		middle?: != "" // optional, but must be non-empty when specified
		last:    != ""
	}

	// The minimum must be strictly smaller than the maximum and vice versa.
	minimum?: int & < maximum
	maximum?: int & > minimum
}

spec: #Spec
spec: {
	kind: "Homo Sapien"
	name: first: "Jane"
	name: last:  "Doe"
}
