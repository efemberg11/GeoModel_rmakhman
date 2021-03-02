# Setup the most basic image
ARG BASEIMAGE=gitlab-registry.cern.ch/jojungge/geomodel:docker_imageBase
FROM ${BASEIMAGE}
ADD .
RUN mkdir -p build  && \
    cd build && \
    cmake ../GeoModelXML && \
    make -j2 && \
    sudo make install
 