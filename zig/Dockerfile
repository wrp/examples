
FROM debian-@user@-dev

ARG commit=c41ac8f

ARG project=zig
ARG url=https://github.com/ziglang/zig.git
ARG llvm_version=21


RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list.d/sid.list
RUN printf 'deb http://deb.debian.org/debian experimental main\n' > /etc/apt/sources.list.d/exp.list

RUN apt-get update
RUN aptitude install -y git
RUN aptitude install -y llvm-${llvm_version}
RUN aptitude install -y clang-${llvm_version}
RUN aptitude install -y libclang-cpp${llvm_version}-dev
RUN aptitude install -y libclang-${llvm_version}-dev
RUN aptitude install -y libclang-cpp${llvm_version}-dev
RUN aptitude install -y liblld-${llvm_version}
RUN aptitude install -y liblldb-${llvm_version}
RUN aptitude install -y liblld-${llvm_version}-dev
RUN aptitude install -y liblldb-${llvm_version}-dev
RUN aptitude install -y cmake
RUN aptitude install -y build-essential

RUN mkdir /${project}
WORKDIR /${project}
COPY Makefile ${project}-*.bundle /${project}
RUN rm /${project}/Makefile

RUN git init

RUN if test -f /${project}/${project}-${commit}.bundle; then \
		git fetch ${project}-${commit}.bundle; \
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

CMD ["/bin/bash"]
