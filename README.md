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

### Ubuntu Linux (Bionic, Disco, or Eoan)


**These instructions will install to /usr/local**

First, set the package manager to install the GeoModel software:

```bash
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
```

Then, install all the dependencies. You run the lines listed below, according to the Ubuntu version you are running: 

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

- see <https://gitlab.cern.ch/GeoModelATLAS/DualUsePlugins>
    
    
### macOS (10.14.5 'Mojave' and 10.15 'Catalina')

Here below you will find the instructions to build GeoModelVisualization and all GeoModel dependencies from source, on macOS.

#### Install the dependencies

For the external dependencies, we will install them by using the package manager Homebrew.

```bash
# install external dependencies
brew install cmake eigen doxygen wget boost hg nlohmann-json

# install Qt
brew install qt5
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.zshrc
```

Then, we install the graphics libraries (Coin and SoQt). We set a custom version of them:

```bash
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install simage coin-bb soqt-bb
```

#### Build GeoModelVisualization and the GeoModel software stack

**Note:** These instructions will install everything in a local folder. 
Feel free to remove the string `-DCMAKE_INSTALL_PREFIX=../install` from the CMake commands here below
if you want to install everything into `/usr/local`. 
If you do that, please remember that you might need to run `sudo make install` instead of `make install`,
otherwise you will not have the correct rights to write under the path `/usr/local`.

**For Ubuntu:** Ubuntu comes with an outdated version of `nlohmann`. Thus, a more recent packaged version can be used during 
compilation of `geomodelvisualization` by adding the option `-DGEOMODEL_USE_BUILTIN_JSON=TRUE` in the `cmake` step.


```bash
# build GeoModelCore
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_geomodelcore
cd build_geomodelcore
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelCore
make -j4
make install

# build GeoModelIO
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_geomodelio
cd build_geomodelio
cmake -DCMAKE_INSTALL_PREFIX=../install ../GeoModelIO
make -j4
make install

# build GeoModelVisualization
git clone https://gitlab.cern.ch/GeoModelDev/geomodelvisualization.git
mkdir build_geomodelvis
cd build_geomodelvis
cmake -DCMAKE_INSTALL_PREFIX=../install ../geomodelvisualization
make -j4
make install
```

After compilation, you should apply two temporary fixes:

On MacOS:
```
install_name_tool -add_rpath ../install/lib ../install/bin/gmex  # this is a temporary fix
```
On Ubuntu:
```
export LD_LIBRARY_PATH=${PWD}/../install/lib/
```
On both:
```
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
```

Then, you can run your local copy of `gmex` with:

```
../install/bin/gmex
```


