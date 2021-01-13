FROM  ubuntu:20.04

USER root
WORKDIR /root
ENV DEBIAN_FRONTEND noninteractive

### Install tzdata package
### During ordinary installation, the package
### wants to be configured. The additional commands
### circumvene the configuration on docker
RUN apt-get update && apt-get install -y tzdata && ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime && dpkg-reconfigure --frontend noninteractive tzdata

### Install C++, cmake and python
RUN apt-get update && apt-get install -y cmake g++ python

### Boost installation
RUN apt-get update && apt-get install -y libboost-all-dev

### Now we come to QT5 as a prerequiste of GeoModel
RUN  apt-get update &&  apt-get install -y qt5-default qt5-qmake qtbase5-dev-tools qt5-doc 

### Next install sqlite for the GeoModel outputs
RUN apt-get update && apt-get install -y sqlite3 libsqlite3-dev

### Last but very important, the installation of the OpenGL module
RUN apt-get update && apt-get install -y mesa-utils build-essential libgl1-mesa-dev

# Set up the ATLAS user, and give it super user rights.
RUN echo '%wheel	ALL=(ALL)	NOPASSWD: ALL' >> /etc/sudoers && \
    adduser atlas && chmod 755 /home/atlas && \
    usermod -aG wheel atlas && \
    usermod -aG root atlas && \
    mkdir /workdir && chown "atlas:atlas" /workdir && \
    chmod 755 /workdir

USER atlas
WORKDIR /workdir

COPY . /workdir/GeoModel_src/

RUN mkdir -p /workdir/build_geomodel/ && \
    cd /workdir/build_geomodel/ && \
    cmake -DCMAKE_INSTALL_PREFIX=../install/ -DGEOMODEL_BUILD_VISUALIZATION=1 -DGEOMODEL_USE_BUILTIN_JSON=1 -DGEOMODEL_USE_BUILTIN_EIGEN3=1 -DGEOMODEL_BUILD_TOOLS=1 -DGEOMODEL_USE_BUILTIN_XERCESC=1 -DGEOMODEL_USE_BUILTIN_COIN3D=1 ../GeoModel_src/ && \
    make -j2 && \
    make install
