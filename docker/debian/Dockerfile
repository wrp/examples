
FROM debian
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN apt-get update
RUN apt-get -y install aptitude
RUN aptitude install -y sudo
RUN aptitude install -y less
RUN aptitude install -y manpages
RUN aptitude install -y man-db
RUN aptitude install -y vim
RUN aptitude install -y perl
RUN aptitude install -y gcc
RUN aptitude install -y procps
RUN aptitude install -y apt-file
RUN aptitude install -y openssl
RUN aptitude install -y libopensc-openssl
RUN aptitude install -y libcrypto++-dev
RUN aptitude install -y libssl-dev
RUN aptitude install -y autoconf
RUN aptitude install -y automake
RUN aptitude install -y libtool
RUN aptitude install -y check
RUN aptitude install -y make
RUN aptitude install -y nmap
RUN aptitude install -y ruby-dev build-essential
RUN apt update
RUN apt-file update
RUN gem install fpm
RUN aptitude install -y git
RUN aptitude install -y rpm
RUN aptitude install -y collectd collectd-utils
RUN aptitude install -y curl
RUN aptitude install -y python3
RUN aptitude install -y python3-yaml
RUN aptitude install -y python-yaml

RUN aptitude install -y dtach
RUN aptitude install -y tmux

COPY .tmux.conf /root
RUN aptitude install -y locales
COPY locale.gen /etc
RUN locale-gen


# RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list
# RUN apt-get update
# RUN aptitude install -y prometheus=2.0.0+ds3-1

ADD sudoers /etc/sudoers
RUN adduser william
CMD bash

EXPOSE 8125
