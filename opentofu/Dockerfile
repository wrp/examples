
ARG user
FROM debian-${user}-dev

ARG user
ARG target_dir=/opentofu
ARG git_url=https://github.com/opentofu/opentofu.git
ENV target_dir=${target_dir}/

RUN mkdir ${target_dir}
COPY Dockerfile opentofu*.bundle ${target_dir}
RUN chown -R $user ${target_dir}
RUN apt-get update
RUN apt-get install -y git

WORKDIR ${target_dir}
USER ${user}

RUN if test -f ${target_dir}*.bundle; then \
	cd ${target_dir} && git init && git fetch *.bundle \
	&& git checkout -b main FETCH_HEAD; \
	else \
		git clone ${git_url}; \
	fi

RUN rm ${target_dir}Dockerfile ${target_dir}opentofu*.bundle
RUN go build -ldflags "-w -s -X 'github.com/opentofu/opentofu/version.dev=no'" -o bin/ ./cmd/tofu

ENV PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opentofu/bin
ENV DOCKER=debian-${user}-dev

CMD /bin/bash
