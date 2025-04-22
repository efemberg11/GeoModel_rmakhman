[![pipeline status](https://gitlab.cern.ch/GeoModelDev/GeoModelExamples/badges/main/pipeline.svg)](https://gitlab.cern.ch/GeoModelDev/GeoModelExamples/commits/main)

# GeoModelExamples

GeoModelExamples contains example programs showing how to use the GeoModel libraries and tools.

Please, refer to the instructions contained in the single subfolders to build specific examples.

## Dependencies

### Qt5

Please, refer to the notes in the Appendix, at the bottom of this page.

### GeoModelCore

You can install the packaged version of the GeoModelCore library by following the instruction in the package repository:

<https://gitlab.cern.ch/GeoModelDev/GeoModelCore>

Alternatively, and **only if** you are interested in developing the base libraries or a packaged version is not available for your system, you can build a fresh local copy from the HEAD by using the CMake's `CMAKE_INSTALL_PREFIX` variable, as below:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
```


### GeoModelIO

You can install the packaged version of the GeoModelCore library by following the instruction in the package repository:

<https://gitlab.cern.ch/GeoModelDev/GeoModelCore>


Alternatively, and **only if** you are interested in developing the base libraries or a packaged version is not available for your system, you can build a fresh local copy from the HEAD by using the CMake's `CMAKE_INSTALL_PREFIX` variable, as below:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO.git
mkdir build_gmio
cd build_gmio
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelIO
make -j 4
make install
```



## Build

You can build all the examples together by issuing the command below from your work folder. If you built a local version of the GeoModelCore and GeoModelIO libraries, then they will be taken into account during the build (because of the `-DCMAKE_INSTALL_PREFIX=../install` statement).


```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples.git
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples
make -j4
```

You can also build a single example, by addressing its source folder instead of the top one, as in the example below:

```
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/HelloGeoWrite
```

## Run

You can run the example by launching them from their "build" directory, for example:

```
./HelloGeoWrite/hellogeoWrite
```

## Visualization

You can visualize it by opening it with VP1Light. Please refer to the [Visualization documentation](docs/visualization.md).



----

## Appendix

### Notes on Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, in the GeoModelIO repository](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/blob/main/README_QT5_NOTES.md).
