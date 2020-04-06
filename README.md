# GeoModelTools 

Tools and utilities for GeoModel-based detector description projects. This includes the CLHEP ExpressionEvaluator 
(which was moved here to use throughout the project without linking against CLHEP), an XML parser (GeoModelXMLParser) and
a JSON Parser. The GeoModelTools installation requires the GeoModel suite ([GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModelCore), 
[GeoModelIO](https://gitlab.cern.ch/GeoModelDev/GeoModelIO)), [Eigen3](http://eigen.tuxfamily.org/), [XercesC](https://xerces.apache.org/xerces-c/), and [nlohmann_json](https://github.com/nlohmann/json).

**Note**: If you are developing on macOS, you can refer to the [quick instructions](#quick-installation-macos) too, which can be found at the bottom of the page.

## Step-by-step Installation

Detailed installation procedures are available for each and every package, here is a brief and not-exhaustive list of 
installation procedures for many of them, just bear in mind prepackaged versions are already available (e.g. 
`brew install eigen` on Mac). 

Note also that the following procedures install libraries and files under /usr/local, for 
a local installation use the `CMAKE_INSTALL_PREFIX` flag, e.g. `cmake -DCMAKE_INSTALL_PREFIX=../install ...`.
That is especially useful when working on multiple versions of the code, for each version to be self-contained in one single folder.

It is suggested to create a local work directory where all code will be downloaded/built. E.g.

```bash
mkdir geomodel_work; cd geomodel_work
```

### Eigen

```bash
git clone https://github.com/eigenteam/eigen-git-mirror.git
mkdir build_eigen; cd build_eigen
cmake ../eigen-git-mirror
sudo make install
cd ..
```


### XercesC


From http://xerces.apache.org/xerces-c/download.cgi download e.g. `xerces-c-3.2.2.tar.gz`

```bash
tar zxvf xerces-c-3.2.2.tar.gz
mkdir build_xercesc; cd build_xercesc
cmake ../xerces-c-3.2.2
make -j
sudo make install
cd ..
```


### nlohmann

```bash
git clone https://github.com/nlohmann/json.git
mkdir build_json; cd build_json
cmake ../json
sudo make install
cd ..
```


### GeoModel tool suite

Note: GeoModelVisualization requires Qt5 and other libraries. Please refer to the `gmex` [documentation](https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization/README.md)


#### Clone and build GeoModelCore from source

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_GeoModelCore; cd build_GeoModelCore
cmake ../GeoModelCore
make -j
sudo make install
cd ..
```

#### Clone and build GeoModelIO

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_GeoModelIO; cd build_GeoModelIO
cmake ../GeoModelIO
make -j
sudo make install
cd ..
```

#### Clone geomodelvisualization and build gmex

```bash
git clone https://gitlab.cern.ch/GeoModelDev/geomodelvisualization.git
mkdir build_geomodelvisualization; cd build_geomodelvisualization
cmake ../geomodelvisualization
make -j
sudo make install
cd ..
```

### Clone and build GeoModelTools

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelTools.git
mkdir build_GeoModelTools; cd build_GeoModelTools
cmake ../GeoModelTools
make -j
sudo make install
cd ..
```

## Quick build instructions - macOS

If you don't have the `brew` package manager installed, please install it by issue the command on its website: https://brew.sh/

```bash
# install external dependencies
brew install eigen xerces-c nlohmann-json qt
# --- configure qt
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.bash_profile

# install GeoModelCore
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_GeoModelCore; cd build_GeoModelCore
cmake ../GeoModelCore
make -j
sudo make install
cd ..

# install GeoModelIO
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_GeoModelIO; cd build_GeoModelIO
cmake ../GeoModelIO
make -j
sudo make install
cd ..

# install GeoModelExplorer ('gmex')
git clone https://gitlab.cern.ch/GeoModelDev/geomodelvisualization.git
mkdir build_geomodelvisualization; cd build_geomodelvisualization
cmake ../geomodelvisualization
make -j
sudo make install
cd ..

# install GeoModelTools
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelTools.git
mkdir build_GeoModelTools; cd build_GeoModelTools
cmake ../GeoModelTools
make -j
sudo make install
cd ..
```

