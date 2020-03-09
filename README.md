# GeoModelXml

This package contains a modernised version of Nigel Hessey's GeoModelXml to be 
used with gmex and the corresponding plugin (GMXPlugin)

## How to clone it

from e.g. your $HOME directory

git clone https://gitlab.cern.ch/GeoModelATLAS/geomodelxml.git

## How to build it

mkdir build_gmx
cd build_gmx
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../geomodelxml
make install

## How to run it

from e.g. your home directory

mkdir run ; cd run
cp ../geomodelxml/GMXPlugin/data/* .
ln -s HelloWorld.gmx gmx.xml
gmex ../install/lib/libGMXPlugin.so  (or .dylib on mac)

 
