# MuonDD

This project contains bits and pieces that were migrated from MuonGeoModel to use AGDD for displaying the NSW. 
In particular, all the NSW files reside here to use them from `gmex`. 

Installation requires that the [GeoModel](https://gitlab.cern.ch/GeoModelDev/GeoModel) suite and AGDD (part of this repository as well) are installed. Please refer to https://gitlab.cern.ch/GeoModelDev/GeoModel/-/blob/master/README.md for additional documentation.

## Clone and build MuonDD

If `GeoModel` and `AGDD` are installed:

```bash
git clone ssh://git@gitlab.cern.ch:7999/atlas/geomodelatlas/GeoModelATLAS.git
mkdir build_geomodelatlas; cd build_geomodelatlas
cmake -DCMAKE_INSTALL_PREFIX=../install/ -DGEOMODELATLAS_BUILD_MUONDD=1 ../GeoModelATLAS
make
sudo make install
cd ..
```

## Running gmex with MuonAGDDPlugin and the NSW example

In case you did not do it already, to run `gmex`, you need:

On MacOS:
```
install_name_tool -add_rpath ../install/lib ../install/bin/gmex
```
On Ubuntu:
```
export LD_LIBRARY_PATH=${PWD}/../install/lib/
```

Running the MuonAGDDPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```
Eventually, although your system language is set to USEnglish, it could still be that your shell language is still set to another language, e.g. when running in a virtual machine on a non-English device. This can be solved by specifying
```bash
export LC_NUMERIC=en_US.UTF-8
```

**NOTE**: If the `GEOMODEL_XML_DIR` environment variable is not set, the `MuonAGDDPlugin` will crash with a segmentation violation, since the `GeoXmlMatManager` cannot find the material definitions. Thus, set this variable to:
```bash
export GEOMODEL_XML_DIR=../GeoModelATLAS/GeoModelDataManagers/GeoXmlMatManager/SampleInput/
```

To display an NSW example xml file (the `.xml` file must be called `AMDC.xml` to use the plugin):
```bash
cd install
cp ../muondd/MuonAGDDPlugin/data/NSW.xml AMDC.xml
bin/gmex lib/libMuonAGDDPlugin.1.0.0.dylib
```

Click on the "Geo" tab, check whatever you please (note that the "New Small Wheel" box groups everything)

## Displaying the ATLAS NSW used in production

When you want to display the real NSW description files which are used in production, have a look at https://gitlab.cern.ch/atlas/athena/-/tree/master/MuonSpectrometer/MuonG4/NSW_Sim/data


