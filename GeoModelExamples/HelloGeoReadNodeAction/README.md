# The 'HelloGeoReadNodeAction' GeoModel example

The `HelloGeoReadNodeAction` example shows you how to read persistified GeoModel data in a standalone program.

The example program:

 1. loads the geometry from the `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them

 The example program uses many of the GeoModel packages.


## Build

To build the GeoModelExamples please refer to the documentation at [GeoModel website](https://geomodel.web.cern.ch/home/)

## Run

Get sample geometry data to play with:

```bash
wget https://geomodel.web.cern.ch/atlas-geometry-data/geometry-ATLAS-R3S-2021-03-02-00.db 
ln -s $PWD/geometry-ATLAS-R3S-2021-03-02-00.db  ../geometry.db
```

Now, you can run the example by typing:

```bash
./hellogeoReadNodeAction geometry.db
```
