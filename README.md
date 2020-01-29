# GeoModelVisualization

Visualization of GeoModel geometry.  This project contains the code for the GeoModelExplorer, which is a super-lightweight version of VP1(Light), but without event display; i.e. it is a pure geometry visualizer.  

This package is still under ferocious development; we are not releasing pre-build code at this point.  We estimate binary distribution kits for Macintosh and Ubuntu platforms by January 2020. 

## Quick installation instructions (Mac)

```bash
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelexplorer
export GXPLUGINPATH=/usr/local/lib/gxplugins # this is a temporary fix
gmex # or, `gmex -d inputFile.db`
```

## Quick installation instructions (Ubuntu Bionic, Disco, or Eoan)

```bash
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
sudo apt install geomodel-explorer
```

## Running GeoModelExplorer:

You just have to execute the following command:

```bash
gmex [myGeoFile1.db] [myGeoFile2.db]....[myPlugin1.so.*] [myPlugin2.so.*]... 
```
Notes:

(on macos, plugins have the .dylib extension rather than the .so.* extension)

(if you have build gmex from source, you have to set LD_LIBRARY_PATH to include 
 the path where the shared libraries are installed). 

Database files can be downloaded from:

<https://gitlab.cern.ch/GeoModelDev/geometry-data>


## Build from source 

### Ubuntu Bionic, Disco, or Eoan


**These instructions will install to /usr/local**

First, set the package manager to install the GeoModel software:

```bash
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
```

Then, install all the dependencies. You run the lines listed below, according to the Ubuntu version you are running: 
- 
- `sudo apt install libsoqt-dev` **on disco and eoan**
- `sudo apt install libsoqt5-dev, qt5-default, qtbase5-dev, libqt5opengl5-dev` **on bionic**
 
Also, install the geomodel libraries:

- the simplest way:
    - `sudo apt install geomodel-io-dev`

- or, build from source and install
    - see <https://gitlab.cern.ch/GeoModelDev/GeoModelIO>
    - see <https://gitlab.cern.ch/GeoModelDev/GeoModelCore> 
 
Then, checkout the source code (git clone or download) of this package and compile it:

```
cd geomodelvisualization
mkdir build
cd build 
cmake ..
make -j
sudo make install
```
You can build plugins for gmex! 

- see <https://gitlab.cern.ch/GeoModelATLAS>
    
    
### macOS

_Coming soon_

