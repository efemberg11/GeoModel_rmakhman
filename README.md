[![pipeline status](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/badges/master/pipeline.svg)](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/commits/master)

# GeoModelIO

GeoModelIO contains all tools needed to read and write the persistent GeoModel tree data from and to file.



# Quick Installation

On macOS and Ubuntu you can quickly install the pre-compiled library, with all the dependencies.

## macOS

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelio
```

on macOS Mojave, the command above will install pre-compiled binaries, so the installation is quick. On other macOS versions, `brew` will build the library from its source code.


## Ubuntu

_coming soon_

----

# Build

If you want to work on the library code, you have to build it by yourself.

First, install or build the [GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModelCore) dependency, by following [its instructions](https://gitlab.cern.ch/GeoModelDev/GeoModelCore/blob/master/README.md).

Then, get the code of this library and compile it:

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_geomodelio
cd build_geomodelio
cmake ../GeoModelIO
make
```

## Local 'install' path

If you want to build a custom version of the library to be used to develop client code, 
you can create a self-contained area by defining a local `install` path; 
you can do it by adding the option `-DCMAKE_INSTALL_PREFIX=../install` when running CMake:

```
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelIO
make
make install
```

The `make install` command will install the headers and the shared libraries into the `../install` folder.
Then, use the same `-DCMAKE_INSTALL_PREFIX=../install` when compiling your client code, to let it find the custom version of the GeoModelIO library.
