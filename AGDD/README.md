# AGDD

This is the set of libraries for the ATLAS AGDD system (muon passive material implementation), including the 
plugin (`AGDDPlugin`) needed to run it with `geomodelexplorer` (`gmex`).

Installation requires that the [GeoModel](https://gitlab.cern.ch/GeoModelDev/GeoModel) suite and [GeoModelDataManagers](https://gitlab.cern.ch/GeoModelATLAS/GeoModelDataManagers) are installed. Please refer to https://gitlab.cern.ch/GeoModelDev/GeoModel/-/blob/master/README.md and https://gitlab.cern.ch/GeoModelATLAS/GeoModelDataManagers/-/blob/master/README.md for additional documentation.

## Installation of GeoModelDataManagers

```bash
git clone https://gitlab.cern.ch/GeoModelATLAS/GeoModelDataManagers
mkdir build_datamanagers && cd build_datamanagers
cmake -DCMAKE_INSTALL_PREFIX=../install/ ../GeoModelDataManagers/
make
make install
cd ..
```

## Cloning and building

If `GeoModel` and `GeoModelDataManagers` are installed:

```bash
git clone https://gitlab.cern.ch/GeoModelATLAS/agdd.git
mkdir build_agdd ; cd build_agdd
cmake  -DCMAKE_INSTALL_PREFIX=../install/ ../agdd
make install
cd ..
```

## Running gmex with AGDD

Running the AGDDPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```
Eventually, although your system language is set to USEnglish, it could still be that your shell language is still set to another language, e.g. when running in a virtual machine on a non-English device. This can be solved by specifying
```bash
export LC_NUMERIC=en_US.UTF-8
```

**NOTE**: If the `GEOMODEL_XML_DIR` environment variable is not set, the `AGDDPlugin` will crash with a segmentation violation, since the `GeoXmlMatManager` cannot find the material definitions. Thus, set this variable to:
```bash
export GEOMODEL_XML_DIR=../GeoModelDataManagers/GeoXmlMatManager/SampleInput/
```

To display an AGDD example xml file:
```bash
cd install
cp ../agdd/AGDDPlugin/data/AMDC.xml .
bin/gmex lib/libAGDDPlugin.1.0.0.dylib
```

Then click on the "Geo" tab, check whichever checkbox you please.

## Displaying the ATLAS passive materials used in production

When you want to display the real AGDD passive material description files which are used in production, have a look at https://gitlab.cern.ch/atlas/athena/-/tree/master/MuonSpectrometer/MuonG4/MuonPassiveMaterials/data



