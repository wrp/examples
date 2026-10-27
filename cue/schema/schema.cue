// This schema will be used to validate a config
#User: {
	id:   int
	name: string
	role: "admin" | "member" // Must be one of these two strings
	age?: int & >35           // Optional field, but must be an integer if provided
}
