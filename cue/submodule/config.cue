package pkg_name

#Config: {  // define the schema for the configuration.
	appName!: string  // the name of the application.
	port!: int  // the port on which the application listens.
	debug?: bool
	features?: {  // optional feature settings
		logging?: bool
		analytics?: bool
	}
}
