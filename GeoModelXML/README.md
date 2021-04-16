# GeoModelXml

This package contains a modernised version of Nigel Hessey's GeoModelXml to be 
used with `gmex` (GeoModelExplorer) and the corresponding plugin (GMXPlugin)

Installation requires that the [GeoModel](https://gitlab.cern.ch/GeoModelDev/GeoModel) suite is installed. Please refer to https://gitlab.cern.ch/GeoModelDev/GeoModel/-/blob/master/README.md for additional documentation.


## Nigel's documentation

https://twiki.atlas-canada.ca/pub/AtlasCanada/ITk/gmx2geo.pdf

## How to build it

If `GeoModel` is installed:

```
# Clone
git clone ssh://git@gitlab.cern.ch:7999/atlas/geomodelatlas/GeoModelATLAS.git

# Configure, build, install locally:
mkdir build_geomodelatlas; cd build_geomodelatlas
cmake -DCMAKE_INSTALL_PREFIX=../install/ -DGEOMODELATLAS_BUILD_GEOMODELXML=1 ../GeoModelATLAS
make
make install

# EXTRA STEP: Add a rpath, only needed when using a local 'install' folder 
install_name_tool -add_rpath ../install/lib ../install/lib/libGMXPlugin.dylib

cd ..
```


## How to run it

Running the GMXPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```

From e.g. your home directory

```
cd install
export GMX_FILES="../GeoModelATLAS/GeoModelXML/data/example1.xml"
bin/gmex lib/libGMXPlugin.1.0.0.dylib (or .so on Linux)
```

Once `gmex` is open, click on the **Geo** tab, then check **DisplacedTube** and click on the "Eye" icon.


You can also display several `.xml` files at once by exporting a space separated string:

```
cd install
export GMX_FILES="../GeoModelATLAS/GeoModelXml/data/example1.xml ../GeoModelATLAS/GeoModelXml/data/example2.xml ../GeoModelATLAS/GeoModelXml/data/example3.xml"
bin/gmex lib/libGMXPlugin.1.0.0.dylib (or .so on Linux)
```

Then, you see 3 items showing up in the **Geo** tab.
