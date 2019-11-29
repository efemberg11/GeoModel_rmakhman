# CC7 OS capable of using/running the ATLAS software release(s).

# Make the base image configurable:
ARG BASEIMAGE=cern/cc7-base:latest

# Set up the CC7 "ATLAS OS":
FROM ${BASEIMAGE}

# Perform the installation as root:
USER root
WORKDIR /root

# Put the repository configuration file(s) in place:
COPY *.repo /etc/yum.repos.d/
# Copy the prompt setup script in place:
COPY atlas_prompt.sh /etc/profile.d/

# 1. Install extra CC7 and LCG packages needed by the ATLAS release
# 2. Install CMake
RUN yum -y install which git wget tar atlas-devel \
      redhat-lsb-core libX11-devel libXpm-devel libXft-devel libXext-devel \
      openssl-devel glibc-devel rpm-build nano sudo \
      gcc_8.3.0_x86_64_centos7 binutils_2.30_x86_64_centos7 \
    && yum clean all \
    && wget https://cmake.org/files/v3.14/cmake-3.14.6-Linux-x86_64.tar.gz \
    && tar -C /usr/local --strip-components=1 --no-same-owner \
      -xvf cmake-3.14.6-Linux-x86_64.tar.gz \
    && rm cmake-3.14.6-Linux-x86_64.tar.gz

# Set up the ATLAS user, and give it super user rights.
RUN echo '%wheel	ALL=(ALL)	NOPASSWD: ALL' >> /etc/sudoers && \
    adduser atlas && chmod 755 /home/atlas && \
    usermod -aG wheel atlas && \
    mkdir /workdir && chown "atlas:atlas" /workdir && \
    chmod 755 /workdir

# LH: Make Images Grid / Singularity compatible
# Note that this is a hack as long as overlayfs
# is not widely on grid sites using singularity
# as a container runtime. For overlay-enabled
# runtimes (such as docker) missing mountpoints
# are created on demand. I hope we can remove this soon.
RUN mkdir -p /alrb /cvmfs /afs /eos

# Add basic usage instructions for the image:
COPY motd /etc/

# Start the image with BASH by default, after having printed the message
# of the day.
CMD cat /etc/motd && /bin/bash
