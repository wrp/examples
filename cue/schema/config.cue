package config

// Define a strict reusable schema
#EnvConfig: {
	name:  string
	debug: bool | *false // Default to false
	replicas: int & >=1  // Constrained integer

	database: {
		host: string
		port: int | *5432
	}
}

// Define the individual environment values
// Each automatically inherits and validates against the schema
environments: [Name=string]: #EnvConfig & {name: Name}
environments: {
	dev: {
		debug:    true
		replicas: 1
		database: host: "localhost"
	}
	staging: {
		replicas: 2
		database: host: "stage-db.internal"
	}
	prod: {
		replicas: 5
		database: host: "prod-db.internal"
		database: port: 1234
	}
}
