
cc_binary(
	name = "hello",
	srcs = ["hello.c"],
)

cc_binary(
	name = "cat",
	srcs = ["cat.c"],
	deps = [ "//:xutil", ],
)

cc_binary(
	name = "calc",
	srcs = ["calc.c", ],
	deps = [ "//:stack", "//:ringbuf", ],
)
