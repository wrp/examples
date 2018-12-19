
FROM debian
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN printf 'bind -v\n' > /root/.editrc
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
RUN aptitude install -y rpm
RUN aptitude install -y collectd collectd-utils
RUN aptitude install -y curl
RUN aptitude install -y python3
RUN aptitude install -y python3-yaml
RUN aptitude install -y python-yaml
RUN aptitude install -y dtach
RUN aptitude install -y tmux
RUN aptitude install -y ansible
RUN aptitude install -y ssh
RUN aptitude install -y ncdu
RUN aptitude install -y git
RUN aptitude install -y strace
RUN aptitude install -y openjdk-8-jdk
RUN aptitude install -y maven


#COPY .tmux.conf /root
#RUN aptitude install -y locales
#COPY locale.gen /etc
#RUN locale-gen


RUN printf 'deb http://deb.debian.org/debian sid main\n' > /etc/apt/sources.list
RUN apt-get update
# RUN aptitude install -y git=1:2.18.0-1
#RUN aptitude install -y prometheus=2.0.0+ds3-1

# Setup to install docker
RUN aptitude install -y apt-transport-https ca-certificates curl gnupg2 \
	software-properties-common
RUN curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -
RUN add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian \
      $(lsb_release -cs) stable"
RUN apt-get update
RUN aptitude install -y docker-ce
RUN aptitude install -y rlwrap
RUN aptitude install -y zlib1g-dev
RUN aptitude install -y exuberant-ctags
RUN aptitude install -y jq

# Fix locales for tmux
RUN aptitude install -y locales
RUN echo 'en_US.UTF-8 UTF-8' > /etc/locale.gen
RUN locale-gen

ENV DOCKER=1
ENV DOCKER_HOST=tcp://10.1.2.2:2376
ENV DOCKER_CERT_PATH=/path/to/cdk/.vagrant/machines/default/virtualbox/.docker
ENV DOCKER_TLS_VERIFY=1

ADD sudoers /etc/sudoers
RUN adduser --quiet docker_user
CMD sudo DOCKER=1 -u docker_user bash

EXPOSE 8125
