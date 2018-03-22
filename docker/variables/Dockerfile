
FROM debian
RUN echo 'single foo is ${foo}' >> /tmp/file
RUN echo "double quoted foo is ${foo}" >> /tmp/file
RUN echo unquoted foo is ${foo} >> /tmp/file
RUN echo defining foo in Dockerfile >> /tmp/file
ARG foo=default
RUN echo 'single foo is ${foo}' >> /tmp/file
RUN echo "double quoted foo is ${foo}" >> /tmp/file
RUN echo unquoted foo is ${foo} >> /tmp/file

ARG v=5
ARG version=1.4.$v

RUN echo version is $version >> /tmp/file

CMD cat /tmp/file

