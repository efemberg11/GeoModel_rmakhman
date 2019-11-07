# GeoModelVisualization

Visualization of GeoModel geometry.  This project contains the code for the GeoModelExplorer, which is a super-lightweight version of VP1(Light), but without event display; i.e. it is a pure geometry visualizer.  

This package is still under ferocious development; we are not releasing pre-build code at this point.  We estimate binary distribution kits for Macintosh and Ubuntu platforms by January 2020. 

# Quick installation instructions (Ubuntu Bionic, Disco, or Eoan)

sudo add-apt-repository ppa:kaktusjoe/geomodel

sudo apt-get update

sudo apt install geomodel-explorer

# Running GeoModelExplorer:

You just have to execute the following command;( database files can be downloaded from ....? )

gmex [-d myGeoFile.db]


# Build from source (Ubuntu Bionic, Disco, or Eoan)


**These instructions will install to /usr/local**

sudo add-apt-repository ppa:kaktusjoe/geomodel

sudo apt-get update

sudo apt install geomodel-io-dev libsoqt-dev** [on disco and eoan]**

sudo apt install libsoqt5-dev, geomodel-io-dev, qt5-default, qtbase5-dev, libqt5opengl5-dev **[on bionic]**

**Then, checkout the source code (git clone or download)**

cd geomodelvisualization

mkdir build

cd build 

cmake ..

make -j

sudo make install




