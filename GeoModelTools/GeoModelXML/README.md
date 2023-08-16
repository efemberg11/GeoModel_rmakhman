# GeoModelXml

This package contains a modernised version of Nigel Hessey's GeoModelXml to be 
used with `gmex` (GeoModelExplorer) and the corresponding plugin (GMXPlugin)

Installation requires that the [GeoModel](https://gitlab.cern.ch/GeoModelDev/GeoModel) suite is installed. Please refer to https://gitlab.cern.ch/GeoModelDev/GeoModel/-/blob/main/README.md for additional documentation.


## Nigel's documentation

https://twiki.atlas-canada.ca/pub/AtlasCanada/ITk/gmx2geo.pdf

## How to build it

`GEoModelXML` is part of the `GeoModel` toolsuite, so it is built as part of the `GeoModelTools`: 

```bash
# Clone
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModel.git

# Configure, build, install locally:
mkdir build
cd build

# ** Choose your installation mode **
# local installation, useful for testing and developing different versions
cmake -DCMAKE_INSTALL_PREFIX=../install/ -DGEOMODEL_BUILD_TOOLS=1 ../GeoModel 
# global installation (in /usr/local/), useful for end users
#cmake -DGEOMODEL_BUILD_TOOLS=1 ../GeoModel 

make -j4
make install

cd ..
```

### Post-build setup [macOS ONLY!]

**NOTE:** This is needed **on macOS only**, and only when using a **local** `install` folder (instead of system folders like `/usr/local/`). 

This extra step tells the `GMXPlugin` where it can find the other libraries, when loaded as a plugin at runtime:

```bash
# EXTRA STEP: Add a rpath, only needed on macOS and when using a local 'install' folder 
install_name_tool -add_rpath install/lib install/lib/libGMXPlugin.dylib
install_name_tool -add_rpath install/lib install/bin/gmex
```

## How to run it

Running the GMXPlugin executes the `Xerces-C` parser to parse the XML file. The expression evaluator only works if the system language is set to USEnglish, thus do:

```bash
export LANG=en_US.UTF-8
```

From e.g. your home directory

```bash
cd install
cp ../GeoModel/GeoModelTools/GeoModelXML/GeoModelXml/data/geomodel.dtd ../GeoModel/GeoModelTools/GeoModelXML/data
export GMX_FILES="../GeoModel/GeoModelTools/GeoModelXML/data/example1.xml"
bin/gmex lib/libGMXPlugin.dylib # (or .so on Linux)
```

Once `gmex` is open, click on the **Geo** tab, then check **DisplacedTube** and click on the "Eye" icon.


You can also display several `.xml` files at once by exporting a space separated string:

```bash
cd install
export GMX_FILES="../GeoModelATLAS/GeoModelXml/data/example1.xml ../GeoModelATLAS/GeoModelXml/data/example2.xml ../GeoModelATLAS/GeoModelXml/data/example3.xml"
bin/gmex lib/libGMXPlugin.1.0.0.dylib (or .so on Linux)
```

Then, you see 3 items showing up in the **Geo** tab.
