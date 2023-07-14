# The 'helloGeoRead' GeoModel example

The `helloGeoRead` example shows you how to read persistified GeoModel data in a standalone program.

The example program:

 1. loads the geometry from the `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them

 The example program uses many of the GeoModel packages.


## Dependencies

### Install Qt5

To build and run GeoModel libraries, you must have a working Qt5 installation on your computer.
If you are not sure how to install it, please take a look at the Appendix on Qt5, below.

### Build GeoModelCore

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmcore
cd build_gmcore
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
```

### Build GeoModelIO

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
mkdir build_hellogeoRead
cd build_hellogeoRead
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelExamples/HelloGeoRead
make -j4
make install
```

## Run

Get sample geometry data to play with:

```bash
wget https://geomodel.web.cern.ch/atlas-geometry-data/geometry-ATLAS-R3S-2021-03-02-00.db 
ln -s $PWD/geometry-ATLAS-R3S-2021-03-02-00.db  ../geometry.db
```

Now, you can run the example by typing:

```bash
./hellogeoRead geometry.db
```


 ----

## Appendix

### Notes on Qt5

To build and run GeoModel I/O libraries, you must have a working Qt5 installation on your computer. Qt5 classes are used for I/O operations with the underlying SQLite daemon, to store/handle data, and to handle logging.

If you are not sure how to install it, please take a look at [the notes on Qt5, in the GeoModelIO repository](https://gitlab.cern.ch/GeoModelDev/GeoModelIO/blob/master/README_QT5_NOTES.md).
