
# Use 'docker search' to find base images
FROM fedora

RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN yum install -y gcc
RUN yum install -y redhat-rpm-config
RUN yum install -y ruby-devel
RUN gem install fpm

ADD sudoers /etc/sudoers
RUN adduser docker_user
CMD bash

EXPOSE 8125
