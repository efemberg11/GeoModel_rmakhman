# MuonDD

This project contains bits and pieces that were migrated from MuonGeoModel and to use AGDD. 
In  particular, all the NSW files reside here to use them from `gmex`. This project must be built
on top of the [GeoModelVisualization](https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization) tool suite and on [AGDD](https://gitlab.cern.ch/GeoModelATLAS/AGDD/). Please refer to the AGDD documentation 
for additional details:

https://gitlab.cern.ch/GeoModelATLAS/agdd/-/blob/master/README.md

## Clone and build MuonDD

From a work directory:

```bash
git clone https://gitlab.cern.ch/GeoModelATLAS/muondd.git
mkdir build_muondd; cd build_muondd
cmake ../muondd
make -j
sudo make install
cd ..
```

## Running gmex with MuonAGDDPlugin and the NSW example

```bash
mkdir run; cd run
cp ../muondd/MuonAGDDPlugin/data/NSW.xml .
ln -s NSW.xml AMDC.xml
gmex /usr/local/lib/libMuonAGDDPlugin.1.0.0.dylib
```

click on the "Geo" tab, check whatever you please (note that the "New Small Wheel" box groups everything)

