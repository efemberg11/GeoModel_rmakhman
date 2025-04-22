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


## Build

To build the GeoModelExamples please refer to the documentation at [GeoModel website](https://geomodel.web.cern.ch/home/)

## Run

Now, you can **run the example** by typing:

```bash
./hellogeo
```
