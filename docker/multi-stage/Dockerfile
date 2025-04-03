ARG sample_arg

FROM debian as base

COPY . /tmp/src

WORKDIR /tmp/src
RUN echo 'this is executed in /tmp/src' > build-output

FROM base as second-image

WORKDIR /tmp/other
COPY --from=base /tmp/src/build-output /tmp/copied-output

# Create /tmp/other/out
RUN sed -e 's/executed/banana/' /tmp/copied-output > out

ENTRYPOINT [ "/bin/bash" ]
