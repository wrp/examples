
FROM debian
COPY a.sh /tmp
RUN /tmp/a.sh
ENV abc=hello
# Environment is set for all subsequent RUN commands and CMD
RUN /tmp/a.sh


CMD /tmp/a.sh
