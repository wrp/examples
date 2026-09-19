

hello: hello.pcm hello.cppm main.cpp
	clang++ -std=c++20 main.cpp hello.cppm -fmodule-file=hello_utils=hello.pcm -o hello


hello.pcm:
	clang++ -std=c++20 -x c++-module hello.cppm --precompile -o hello.pcm
