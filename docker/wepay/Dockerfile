
FROM artifactory.devops.wepay-inc.com/wepay/python:3.1.1
RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN yum install -y sudo
RUN yum install -y gcc

ADD sudoers /etc/sudoers
RUN adduser docker_user
CMD bash

EXPOSE 8125
