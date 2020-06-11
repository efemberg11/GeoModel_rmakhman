# AGDD

This is the set of libraries for the ATLAS AGDD system, including the 
plugin (`AGDDPlugin`) needed to run it with `gmex`.

Installation requires that the [GeoModelVisualization](https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization) suite as well as [GeoModelTools](https://gitlab.cern.ch/GeoModelDev/GeoModelTools) be installed. Please refer to 

https://gitlab.cern.ch/GeoModelDev/GeoModelTools/-/blob/master/README.md

for additional documentation.

## Cloning and building. 

From e.g. a word directory.

```bash
git clone https://gitlab.cern.ch/GeoModelATLAS/agdd.git
mkdir build_agdd ; cd build_agdd
cmake  ../agdd
sudo make install
cd ..
```

## Running gmex with AGDD

Running the AGDDPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```

```bash
mkdir run; cd run
cp ../agdd/AGDDPlugin/data/AMDC.xml .
gmex /usr/local/lib/libAGDDPlugin.1.0.0.dylib
```

Then click on the "Geo" tab, check whichever checkbox you please.

