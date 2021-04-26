# AGDD

This is the set of libraries for the ATLAS AGDD system (muon passive material implementation), including the 
plugin (`AGDDPlugin`) needed to run it with `geomodelexplorer` (`gmex`).

Installation requires that the [GeoModel](https://gitlab.cern.ch/GeoModelDev/GeoModel) suite is installed. Please refer to https://gitlab.cern.ch/GeoModelDev/GeoModel/-/blob/master/README.md for additional documentation.

## Cloning and building

If `GeoModel` is installed:

```bash
git clone ssh://git@gitlab.cern.ch:7999/atlas/geomodelatlas/GeoModelATLAS.git
mkdir build_geomodelatlas; cd build_geomodelatlas
cmake -DCMAKE_INSTALL_PREFIX=../install/ -DGEOMODELATLAS_BUILD_AGDD=1 ../GeoModelATLAS
make
sudo make install
cd ..
```

## Running gmex with AGDD

In case you did not do it already, to run `gmex`, you need:

On MacOS:
```
install_name_tool -add_rpath ../install/lib ../install/bin/gmex
```
On Ubuntu:
```
export LD_LIBRARY_PATH=${PWD}/../install/lib/
```

Running the AGDDPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```
Eventually, although your system language is set to USEnglish, it could still be that your shell language is still set to another language, e.g. when running in a virtual machine on a non-English device. This can be solved by specifying
```bash
export LC_NUMERIC=en_US.UTF-8
```

**NOTE**: If the `GEOMODEL_XML_DIR` environment variable is not set, the `AGDDPlugin` will crash with a segmentation violation, since the `GeoXmlMatManager` cannot find the material definitions. Thus, set this variable by doing:
```bash
cd install
mkdir Materials
cp ../GeoModelATLAS/GeoModelDataManagers/GeoXmlMatManager/SampleInput/* Materials/
export GEOMODEL_XML_DIR=${PWD}
```

To display an AGDD example xml file (the `.xml` file must be called `AMDC.xml` to use the plugin):
```bash
cd install
cp ../GeoModelATLAS/AGDD/AGDDPlugin/data/AMDC.xml .
bin/gmex lib/libAGDDPlugin.1.0.0.dylib
```

Then click on the "Geo" tab, check whichever checkbox you please.

## Displaying the ATLAS passive materials used in production

When you want to display the real AGDD passive material description files which are used in production, have a look at https://gitlab.cern.ch/atlas/athena/-/tree/master/MuonSpectrometer/MuonG4/MuonPassiveMaterials/data



