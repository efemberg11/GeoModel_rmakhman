# The 'HelloToyDetectorFactory' GeoModel example

The `HelloToyDetectorFactory` example shows you how to create a complex geometry using the GeoModel DetectorFactory and Scrutinizers.

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
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelExamples/HelloToyDetectorFactory/
make -j4
```

## Run

Now, you can **run the example** by typing:

```bash
./helloToyDetectorFactory
```

### Notes

The example program:

 1. Build an example geometry, by using a DetectorFactory
 2. Access the geometry:
   * gets the RootVolume of the GeoModel tree and it prints out the number of its children
   * loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
