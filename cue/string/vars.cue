// Simple example demonstrating string manipulation

import "strings"

#foo: {

	hostname!: string | "db.internal.net" & =~".net$"  // raise error if regex unmatched
	valid:    strings.HasSuffix(hostname, ".foo")  // Evaluates to false, does not raise error

	name: string | *"foo"
	_service: string | *"api"  // leading underscore prevents concrete vaue (won't export)
	deploymentName: "\(name)-\(_service)-v1"

	// The basic mental model in CUE is: Constraint & ( *DefaultValue | AlternativeTypesOrValues )
	port: int & >=128 & < 255 | *128
}

con: C=#foo & {
	hostname: "foo.net"
	name: "alice"
	_assert: true & (C.deploymentName == "alice-api-v1")

	_base: 128
	port: C._base + 10 &== 138
	_assert2: true & (C.port == 138)
}
