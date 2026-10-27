// Simple example demonstrating string manipulation

import "strings"

#foo: {

	hostname!: string | "db.internal.net" & =~".net$"  // raise error if regex unmatched
	valid:    strings.HasSuffix(hostname, ".foo")  // Evaluates to false, does not raise error

	name: string | *"foo"
	_service: string | *"api"  // leading underscore prevents concrete vaue (won't export)
	deploymentName: "\(name)-\(_service)-v1"

	port: 123 & >=123 & < 124

	_assert: true & (port == 123)


	// The basic mental model in CUE is: Constraint & ( *DefaultValue | AlternativeTypesOrValues )
	defaulting_port: int & >=1024 & <2000 | *1058
}

con: C=#foo & {
	hostname: "foo.net"
	name: "alice"
	_assert: true & (C.deploymentName == "alice-api-v1")
}
