
FROM debian
RUN printf 'foo is $foo\n' >> /tmp/file
RUN printf "quoted foo is $foo\n" >> /tmp/file
ARG foo=default
RUN printf 'foo is $foo\n' >> /tmp/file
RUN printf "quoted foo is $foo\n" >> /tmp/file


CMD cat /tmp/file

