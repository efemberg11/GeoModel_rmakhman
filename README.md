# GeoModelTools 

Tools and utilities for GeoModel-based detector description projects. This includes the CLHEP ExpressionEvaluator 
(which was moved here to use throughout the project without linking against CLHEP), an XML parser (GeoModelXMLParser) and
a JSON Parser. The GeoModelTools installation requires the GeoModel suite (GeoModelCore, GeoModelIO), Eigen3, XercesC,
nlohmann.

## Installation

Detailed installation procedures are available for each and every package, here is a brief and not-exhaustive list of 
installation procedures for many of them, just bear in mind prepackaged versions are already available (e.g. 
`brew install eigen` on Mac). Note also that the following procedures install libraries and files under /usr/local, for 
a local installation use the CMAKE_INSTALL_PREFIX flag, e.g. `cmake -DCMAKE_INSTALL_PREFIX=../install ...`

it is suggested to create a local work directory where all code will be downloaded/built. E.g.

mkdir geomodel_work; cd geomodel_work

### Eigen

git clone https://github.com/eigenteam/eigen-git-mirror.git

mkdir build_eigen; cd build_eigen

cmake ../eigen-git-mirror

sudo make install

cd ..

### XercesC

from http://xerces.apache.org/xerces-c/download.cgi download e.g. xerces-c-3.2.2.tar.gz

tar zxvf xerces-c-3.2.2.tar.gz

mkdir build_xercesc; cd build_xercesc

cmake ../xerces-c-3.2.2

make -j

sudo make install

cd ..

### nlohmann

git clone https://github.com/nlohmann/json.git

mkdir build_json; cd build_json

cmake ../json

sudo make install

cd ..

### GeoModel tool suite

Note: GeoModelVisualization requires Qt5 and other libraries. Please refer to the gmex documentation

#### Clone and build GeoModelCore from source

git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git

mkdir build_GeoModelCore; cd build_GeoModelCore

cmake ../GeoModelCore

make -j

sudo make install

cd ..

#### Clone and build GeoModelIO

git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git

mkdir build_GeoModelIO; cd build_GeoModelIO

cmake ../GeoModelIO

make -j

sudo make install

cd ..

#### Clone geomodelvisualization and build gmex

git clone https://gitlab.cern.ch/GeoModelDev/geomodelvisualization.git

mkdir build_geomodelvisualization; cd build_geomodelvisualization

cmake ../geomodelvisualization

make -j

sudo make install

cd ..

### Clone and build GeoModelTools

git clone https://gitlab.cern.ch/GeoModelDev/GeoModelTools.git

mkdir build_GeoModelTools; cd build_GeoModelTools

cmake ../GeoModelTools

make -j

sudo make install

cd ..


