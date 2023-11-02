
cc_binary(
	name = "hello",
	srcs = ["hello.c"],
)

cc_binary(
	name = "cat",
	srcs = ["cat.c"],
	deps = [ "//lib:xutil", ],
)

cc_binary(
	name = "calc",
	srcs = ["calc.c", ],
	deps = [ "//lib:stack", "//lib:ringbuf", ],
	visibility = ["//test:__pkg__"],
)
