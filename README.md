# GeoModelXml

This package contains a modernised version of Nigel Hessey's GeoModelXml to be 
used with `gmex` (GeoModelExplorer) and the corresponding plugin (GMXPlugin)

## Quick Instructions

### How to clone it

From e.g. your $HOME directory

```
git clone https://gitlab.cern.ch/GeoModelATLAS/geomodelxml.git
```

### How to build it

```
mkdir build_gmx
cd build_gmx
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../geomodelxml
make install
```

### How to run it

Running the GMXPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```

From e.g. your home directory

```
mkdir run ; cd run
cp ../geomodelxml/GMXPlugin/data/* .
ln -s HelloWorld.gmx gmx.xml
gmex ../install/lib/libGMXPlugin.so  (or .dylib on mac)
```
 


Then, you can run `gmex` (GeoModelExplorer) with:

```
gmex /usr/local/lib/libGMXPlugin.dylib
```

Once `gmex` is open, click on the **Geo** tab, then check **SCT**



