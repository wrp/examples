# FROM python:3.12-slim-bookworm
FROM python:3.9-slim
RUN pip install numpy
RUN pip install scipy
RUN pip install setuptools
RUN pip install scikit-learn

ARG user
ARG uid
ARG dest_home=/home/$user

RUN adduser --uid $uid --quiet $user || \
	useradd ${user} || \
	adduser -D ${user}

WORKDIR ${dest_home}
ENV DOCKER=numpy-${user}

LABEL description='Image with python tools for numpy, etc.'
CMD ["/bin/bash"]
