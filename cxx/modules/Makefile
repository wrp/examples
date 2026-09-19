

hello: hello.pcm hello.o main.cpp
	clang++ -std=c++20 main.cpp hello.o -fmodule-file=hello_utils=hello.pcm -o hello


hello.pcm:
	clang++ -std=c++20 -x c++-module hello.cppm --precompile -o hello.pcm



hello.o: hello.pcm
	clang++ -std=c++20 -c hello.pcm -o hello.o


clean:
	rm hello hello.pcm hello.o
