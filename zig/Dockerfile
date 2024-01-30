
FROM debian-@user@-dev

ARG project=zig
ARG url=https://github.com/ziglang/zig.git


RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list.d/sid.list
RUN printf 'deb http://deb.debian.org/debian experimental main\n' > /etc/apt/sources.list.d/exp.list

RUN apt-get update
RUN aptitude install -y git
RUN aptitude install -y llvm-17
RUN aptitude install -y clang-17
RUN aptitude install -y libclang-cpp17-dev
RUN aptitude install -y libclang-17-dev
RUN aptitude install -y libclang-cpp17-dev
RUN aptitude install -y liblld-17
RUN aptitude install -y liblldb-17
RUN aptitude install -y liblld-17-dev
RUN aptitude install -y liblldb-17-dev
RUN aptitude install -y cmake
RUN aptitude install -y build-essential

RUN mkdir /${project}
WORKDIR /${project}
copy Makefile ${project}-*.bundle /${project}
RUN rm /${project}/Makefile

RUN git init

RUN if test -f /${project}/*.bundle; then \
		git fetch *.bundle; \
	else \
		git fetch ${url}; \
	fi; \
	git checkout -b main FETCH_HEAD;

RUN mkdir build
WORKDIR /${project}/build

RUN cmake ..
RUN make

RUN mv stage3/bin/zig /usr/local/bin
RUN mv stage3/lib/zig /usr/local/lib
ARG user
USER ${user}

CMD /bin/bash
