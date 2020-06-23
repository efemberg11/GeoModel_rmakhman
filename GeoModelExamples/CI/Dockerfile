FROM cern/slc6-base:latest


# minimal development packages
RUN yum -y update \
    && yum -y install \
        cmake \
        glibc-devel \
        which \
    && yum -y clean all

