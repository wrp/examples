
# My minimal debian image
FROM debian-@user@
RUN apt-get update
RUN apt-get -y install aptitude
RUN mv /etc/sudoers /etc/sudoers-orig
RUN aptitude install -y sudo
RUN mv /etc/sudoers-orig /etc/sudoers
RUN aptitude install -y less
RUN aptitude install -y vim
RUN aptitude install -y perl
RUN aptitude install -y gcc
RUN aptitude install -y make
RUN aptitude install -y nmap
RUN apt-get update
RUN aptitude install -y gdb
RUN aptitude install -y git
RUN aptitude install -y libncurses-dev
RUN aptitude install -y ncurses-doc
RUN aptitude install -y golang
RUN aptitude install -y manpages
RUN aptitude install -y man-db
RUN aptitude install -y cmake
RUN aptitude install -y git
RUN aptitude install -y bazel-bootstrap
RUN aptitude install -y bazel-rules-cc
RUN aptitude install -y yq

ARG user
ARG uid
ARG dest_home=/home/$user

WORKDIR ${dest_home}
ENV DOCKER=debian-${user}
LABEL description='Debian image with developer tools installed'

CMD /bin/bash
