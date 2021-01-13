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
RUN ap-get update && apt-get install -y mesa-utils build-essential libgl1-mesa-dev

