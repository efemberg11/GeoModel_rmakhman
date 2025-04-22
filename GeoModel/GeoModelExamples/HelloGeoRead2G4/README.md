
# The 'hellogeoRead2G4' GeoModel example

The `hellogeoRead2G4` example shows you how to read persistified GeoModel data in a standalone program and convert it to a Geant4 geometry.

This is built on top of the [`hellogeo` example program](https://gitlab.cern.ch/GeoModelDev/GeoModelExamples/helloGeo).

The example program:

 1. loads the geometry specified as input in a `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
 6. transforms the GeoModel tree to Geant4 geometry


## Build

To build the GeoModelExamples please refer to the documentation at [GeoModel website](https://geomodel.web.cern.ch/home/)

## Run `hellogeoRead2G4`

Get sample geometry data to play with:

```bash
wget https://geomodel.web.cern.ch/atlas-geometry-data/geometry-ATLAS-R3S-2021-03-02-00.db 
```
Now, you can run the example by typing:

```bash
./hellogeoRead2G4 geometry-ATLAS-R3S-2021-03-02-00.db
```
