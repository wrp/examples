
FROM centos

RUN printf 'set editing-mode vi\nset keymap vi\n' > /etc/inputrc
RUN yum install -y gcc
RUN yum install -y make
# Grrr, we need to install autotools in order to install fpm
# perhaps we can install libffi instead, but let's avoid that
# rabbit hole for now.
RUN yum install -y autoconf
RUN yum install -y automake
RUN yum install -y libtool
RUN yum install -y redhat-rpm-config
RUN yum install -y ruby-devel
RUN gem install fpm

ADD sudoers /etc/sudoers
RUN adduser william
RUN yum install -y sudo
RUN yum install -y wget
RUN yum install -y rpm-build
RUN yum install -y which
RUN curl https://sdk.cloud.google.com > /tmp/install
RUN chmod +x /tmp/install
RUN /tmp/install --disable-prompts
RUN ln -s /root/google-cloud-sdk/bin/gsutil /bin/gsutil

CMD bash
EXPOSE 8125
