[![pipeline status](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/badges/main/pipeline.svg)](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/commits/main)

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

sudo add-apt-repository ppa:kaktusjoe/geomodel

sudo apt-get update

sudo apt install geomodel-io-dev

----

# Steering the library behavior

## Number of threads

At run time, you can choose to run in serial mode by set this environment variable before running any code built on top of `GeoModelIO`:

```
export GEOMODEL_ENV_IO_NTHREADS=0
```

You can also choose the number of threads for GeoModelIO to use during I/O operations. For example, if you want to use 8 threads, just set:

```
export GEOMODEL_ENV_IO_NTHREADS=8
```

You can also ask `GeoModelIO` to use exactly the number of threads supported by your platform by passing `-1` to the variable, as here below. On a quad-core CPU offering 8 native threads, 8 threads will be used by `GeoModelIO`.

```
export GEOMODEL_ENV_IO_NTHREADS=-1
```


----

# Build

If you want to work on the library code, or if you want to use the latest version of the code, you have to build it by yourself.

First, install or build the [GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModelCore) dependency, by following [its instructions](https://gitlab.cern.ch/GeoModelDev/GeoModelCore/blob/main/README.md).

Then, get the code of this library and compile it:

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_geomodelio
cd build_geomodelio
cmake ../GeoModelIO
make
```

## Build options

### Timing information

#### GEOMODEL_IO_TIMING

You can ask `GeoModelIO` to print timing information, by setting the `GEOMODEL_IO_TIMING` variable while configuring the build with CMake; for example, by using:

```
cmake -DGEOMODEL_IO_TIMING=ON ../GeoModelIO/
```

### Debug information

#### GEOMODEL_IO_DEBUG

You can ask `GeoModelIO` to print debug information, by setting the `GEOMODEL_IO_DEBUG` variable while configuring the build with CMake:

```
cmake -DGEOMODEL_IO_DEBUG=ON ../GeoModelIO/
```

#### GEOMODEL_IO_DEEP_DEBUG

You can ask `GeoModelIO` to print even more debug information, by setting the `GEOMODEL_IO_DEEP_DEBUG` variable while configuring the build with CMake:

```
cmake -DGEOMODEL_IO_DEEP_DEBUG=ON ../GeoModelIO/
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
