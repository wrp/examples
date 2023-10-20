
FROM debian-wrp

ARG project=zig
ARG url=https://github.com/ziglang/zig.git

RUN mkdir /${project}
copy Makefile ${project}-*.bundle /${project}
RUN rm /${project}/Makefile
WORKDIR /${project}
RUN git init
RUN if test -f /${project}/*.bundle; then \
		git fetch *.bundle; \
	else \
		git fetch ${url}; \
	fi; \
	git checkout -b main FETCH_HEAD;

RUN mkdir build
WORKDIR /${project}/build

RUN printf 'deb http://deb.debian.org/debian sid main\n' | sudo tee -a /etc/apt/sources.list.d/sid.list
RUN printf 'deb http://deb.debian.org/debian experimental main\n' | sudo tee -a /etc/apt/sources.list.d/exp.list
RUN sudo apt-get update
RUN sudo aptitude -y install llvm-17
RUN sudo aptitude -y install clang-17
RUN sudo aptitude -y install libclang-cpp17-dev
RUN sudo aptitude -y install libclang-17-dev
RUN sudo aptitude -y install liblld-17
RUN sudo aptitude -y install liblldb-17
RUN sudo aptitude -y install liblld-17-dev
RUN sudo aptitude -y install liblldb-17-dev
RUN cmake ..
RUN make
RUN cp stage3/bin/zig /usr/local/bin
COPY src /src

CMD /bin/bash
