load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
	name = "hello",
	srcs = ["hello.c"],
)

cc_library(
	name = "xutil",
	srcs = ["xutil.c"],
	hdrs = ["xutil.h"],
)

cc_binary(
	name = "cat",
	srcs = ["cat.c"],
	deps = [ ":xutil", ],
)
