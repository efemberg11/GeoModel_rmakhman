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
