[![pipeline status](https://gitlab.cern.ch/GeoModelDev/GeoModelCore/badges/master/pipeline.svg)](https://gitlab.cern.ch/GeoModelDev/GeoModelCore/commits/master)

# GeoModelCore

GeoModelCore contains the core packages of the GeoModel toolkit.



# Quick Installation

On macOS and Ubuntu you can quickly install the pre-compiled library, with all the dependencies.

## macOS

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelcore
```

on macOS Mojave, the command above will install pre-compiled binaries, so the installation is quick. On other macOS versions, `brew` will build the library from its source code.


## Ubuntu (Bionic, Disco, or Eoan)

sudo add-apt-repository ppa:kaktusjoe/geomodel

sudo apt-get update

sudo apt install geomodel-core-dev


----

# Build

If you want to work on the library code, you have to build it by yourself, by getting the source code and compile it.

## Dependencies

- Eigen: http://eigen.tuxfamily.org/

## Build instructions

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_geomodelcore
cd build_geomodelcore
cmake ../GeoModelCore
make
```

## Local 'install' path

If you want to build a custom version of the library to be used to develop client code, 
you can create a self-contained area by defining a local `install` path; 
you can do it by adding the option `-DCMAKE_INSTALL_PREFIX=../install` when running CMake:

```
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelCore
make
make install
```

The `make install` command will install the headers and the shared libraries into the `../install` folder.
Then, use the same `-DCMAKE_INSTALL_PREFIX=../install` when compiling your client code, to let it find the custom version of the GeoModelCore library.
