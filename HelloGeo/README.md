# The 'helloGeo' GeoModel example

The `helloGeo` example shows you how to read persistified GeoModel data in a standalone program.

This example program reads in GeoModel data from a `.db` file, it prints out data entries,
it builds the GeoModel tree, it access the RootVolume, and it loops over all its child volumes, printing the name of the GeoLogVol associated with them.

It uses many of the GeoModel packages.

## Dependencies

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

### Notes

The example program:

 1. Build an example geometry:
   * creates some dummy materials
   * builds a "world" volume
   * builds two additional volumes and adds them to the GeoModel tree as world's children
 2. Access the geometry:
   * gets the RootVolume of the GeoModel tree and it prints out the number of its children
   * loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
