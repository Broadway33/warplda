FROM ubuntu

ARG BE=build-essential
ARG GIT=git
ARG NUMA=libnuma-dev
ARG PYTHON=python2.7
ARG CMAKE=cmake
ARG WGET=wget

RUN apt-get update \
    && apt-get install -y \
       $BE \
       $GIT \
       $PYTHON \
       $CMAKE \
       $NUMA \
       $WGET \
    && apt-get clean -y

RUN git clone https://github.com/Broadway33/warplda

WORKDIR /warplda
RUN ./get_gflags.sh

WORKDIR /warplda/data
RUN wget https://raw.githubusercontent.com/sudar/Yahoo_LDA/master/test/ydir_1k.txt \
    && head -n 900 ydir_1k.txt > ydir_train.txt \
    && tail -n 100 ydir_1k.txt > ydir_test.txt

WORKDIR /warplda
RUN ./build.sh

WORKDIR /warplda/release/src
RUN make -j

ENTRYPOINT cd /warplda/release/src && ../../test.sh
