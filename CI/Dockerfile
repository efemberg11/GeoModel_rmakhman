# Setup the most basic image
ARG BASEIMAGE=gitlab-registry.cern.ch/jojungge/geomodel:docker_imageBase
FROM ${BASEIMAGE}

ADD . /workdir/GeoModelXML
RUN . ~/.bashrc && \
     export  && \
     pwd && \
     cd /workdir && \
     mkdir -p build  && \
     cd build && \
     cmake ../GeoModelXML && \
     make -j2 && \
     sudo make install
 