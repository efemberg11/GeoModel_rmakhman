# The 'geoActions' GeoModel example

The `geoActions` example program shows you how to create a simple geometry, by using GeoModel nodes, and loop over its nodes.

The example program:

 1. Builds an example geometry:
 4. It instantiates a GeoVolumeAction, and uses it to loop over all VPhysVol nodes in the GeoModel tree
 5. It also instantiates a GeoNodeAction, and uses it to loop over all nodes, of any type, in the GeoModel tree
 6. It uses an action to create a filter on tree's nodes, based on regular expressions

## Dependencies

### Install Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at the Appendix, below.


#### GeoModelCore

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
cd ..
```

#### GeoModelIO

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO.git
mkdir build_gmio
cd build_gmio
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelIO
make -j 4
make install
```



## Build

From your work folder:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples.git
mkdir build_geoactions
cd build_geoactions
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/GeoActions/
make -j4
```

## Run

Now, you can **run the example** by typing:

```bash
./geoActions
```


----

## Appendix

### Notes on Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, in the GeoModelIO repository](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/blob/main/README_QT5_NOTES.md).
