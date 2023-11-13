
ARG user
FROM debian-${user}-dev

ARG project=zig
ARG url=https://github.com/ziglang/zig.git

RUN mkdir /${project}
copy Makefile ${project}-*.bundle /${project}
RUN rm /${project}/Makefile
WORKDIR /${project}
RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list.d/sid.list
RUN printf 'deb http://deb.debian.org/debian experimental main\n' > /etc/apt/sources.list.d/exp.list
RUN apt-get update
RUN apt install -y git

RUN git init

RUN if test -f /${project}/*.bundle; then \
		git fetch *.bundle; \
	else \
		git fetch ${url}; \
	fi; \
	git checkout -b main FETCH_HEAD;

RUN mkdir build
WORKDIR /${project}/build

RUN apt install -y llvm-17
RUN apt install -y clang-17
RUN apt install -y libclang-cpp17-dev
RUN apt install -y libclang-17-dev
RUN apt install -y libclang-cpp17-dev
RUN apt install -y liblld-17
RUN apt install -y liblldb-17
RUN apt install -y liblld-17-dev
RUN apt install -y liblldb-17-dev
RUN apt install -y cmake
RUN apt install -y build-essential
RUN cmake ..
RUN make

RUN mv stage3/bin/zig /usr/local/bin
RUN mv stage3/lib/zig /usr/local/lib
ARG user
USER ${user}

CMD /bin/bash
