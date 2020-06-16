# Instructions for building GeoModelKernel library

## External dependency: Eigen

Eigen is the only external dependency of the GeoModelKernel library. If Eigen3 is already available
on your system and CMake can find it with `find_package(Eigen3)`, the you can skip the rest of this section.

There are several ways of installing Eigen on your system. Some of them are OS-dependent (e.g.
`apt-get install libeigen3-dev` on Ubuntu). A generic option is to download Eigen's source code from
http://eigen.tuxfamily.org/index.php?title=Main_Page#Download. The following example demonstrates
how install Eigen version 3.3.4 into ${EIGEN3_INSTALL_DIR}:

``` bash
wget http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
tar -zxf 3.3.4.tar.gz
mv eigen-eigen-* eigen-src
mkdir eigen_build
cd eigen_build
cmake -DCMAKE_INSTALL_PREFIX=${EIGEN3_INSTALL_DIR} ../eigen-src
make install
```
## Build and Install GeoModelKernel

The example shows how to build and install GeoModelKernel at ${GEOMODEL_INSTALL_DIR}.
It assumes that Eigen is already installed on the system at ${EIGEN3_INSTALL_DIR}

``` bash
git clone --recurse-submodules https://gitlab.cern.ch/GeoModelDev/GeoModelKernel.git geomodel_src
mkdir geomodel_build
cd geomodel_build
cmake -DCMAKE_INSTALL_PREFIX=${GEOMODEL_INSTALL_DIR} -DEIGEN3_INCLUDE_DIR=${EIGEN3_INSTALL_DIR}/include/eigen3 ../geomodel_src
make
make install
```

## Build and Install on Mac OS

Installing Eigen3 with Brew and using a local `install` folder for the final installation:

```bash
brew install eigen
git clone --recurse-submodules https://gitlab.cern.ch/GeoModelDev/GeoModelKernel.git geomodel_src
mkdir geomodel_build
cd geomodel_build
cmake -DCMAKE_INSTALL_PREFIX=../install -DEIGEN3_INCLUDE_DIR=/usr/local/include/eigen3 ../geomodel_src
make
make install
```

