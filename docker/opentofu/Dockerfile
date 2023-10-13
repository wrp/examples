
FROM debian-wrp

ARG user=docker_user
ARG target_dir=/opentofu
ARG git_url=https://github.com/opentofu/opentofu.git
ENV target_dir=${target_dir}

RUN mkdir ${target_dir}
COPY Dockerfile opentofu*.bundle ${target_dir}
RUN rm ${target_dir}/Dockerfile
RUN chown -R $user ${target_dir}

WORKDIR ${target_dir}
USER ${user}

RUN if test -f ${target_dir}/*.bundle; then \
	cd ${target_dir} && git init && git fetch *.bundle \
	&& git checkout -b main FETCH_HEAD; \
	else \
		git clone ${git_url}; \
	fi

RUN go build -ldflags "-w -s -X 'github.com/opentofu/opentofu/version.dev=no'" -o bin/ ./cmd/tofu

CMD sudo DOCKER=debian -u "$USER" bash
