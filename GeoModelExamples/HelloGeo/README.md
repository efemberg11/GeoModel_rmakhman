# The 'helloGeo' GeoModel example

The `helloGeo` example shows you how to create a simple geometry, by using GeoModel nodes.

The example program:

 1. Build an example geometry:
   * creates some dummy materials
   * builds a "world" volume
   * builds two additional volumes and adds them to the GeoModel tree as world's children
 2. Access the geometry:
   * gets the RootVolume of the GeoModel tree and it prints out the number of its children
   * loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them


## Dependencies

### Install Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at the Appendix, below.

### GeoModelCore

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
cd ..
```



## Build

From your work folder:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples.git
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/HelloGeo/
make -j4
```

## Run

Now, you can **run the example** by typing:

```bash
./hellogeo
```

----

## Appendix

### Notes on Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, in the GeoModelIO repository](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/blob/master/README_QT5_NOTES.md).
