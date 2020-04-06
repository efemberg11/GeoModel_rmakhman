# AGDD

this is the set of libraries for the ATLAS AGDD system, including the 
plugin (AGDDPlugin) needed to run it with gmex

Installation requires that the GeoModelVisualization suite as well as GeoModelTools be installed. please refer to 

https://gitlab.cern.ch/GeoModelDev/GeoModelTools/-/blob/master/README.md

for additional documentation.

## Cloning and building. 

from e.g. a word directory.

git clone https://gitlab.cern.ch/GeoModelATLAS/agdd.git

mkdir build_agdd ; cd build_agdd

cmake  ../agdd

sudo make install

cd ..

## Running gmex with AGDD

mkdir run; cd run

cp ../agdd/AGDDPlugin/data/AMDC.xml .

gmex /usr/local/lib/libAGDDPlugin.1.0.0.dylib
	
click on the Geo tab, check whichever checkbox you please

