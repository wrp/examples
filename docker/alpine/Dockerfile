
FROM alpine
ARG user=docker_user

RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc
RUN apk add perl-utils

ENV USER=$user
CMD DOCKER=alpine /bin/sh

EXPOSE 8125
