# Build the GeoModel libraries

Here below, you will find instructions on how to compile the GeoModel libraries from scratch. You can want to do that for different reasons:

- you want to build the libraries for an OS which is not officially supported or for a version of it for which no pre-compiled packages are provided (for info, see the [Start](/start/install.md) section)
- you want to test the latest version of the code
- you want to contribute to the development of the libraries

In the following, you will find basic strategies to effectively build the GeoModel libraries, based on different use cases. For more details, you can find updated and additional instructions inside the `README` file of the [GeoModelVisualization/README.md](https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization/README.md) specific package.

## System Dependencies

### Linux/Ubuntu

```bash
apt-get update -qq && apt-get install -y -qq git cmake wget unzip build-essential freeglut3-dev libboost-all-dev qt5-default mercurial libeigen3-dev libxerces-c-dev
```

### macOS

```bash
# install external dependencies
brew install cmake eigen doxygen wget boost hg xerces-c

# install Qt
brew install qt5
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.zshrc
```


## Quick instructions - Build everything


### Build and Install locally

```bash
# Build Coin3D
wget https://bitbucket.org/Coin3D/coin/downloads/coin-4.0.0-src.zip
unzip coin-4.0.0-src.zip -d coin-sources
mv coin-sources/* coin
mkdir build_coin
cd build_coin
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../coin
make -j4
make install
cd ..

# Build SoQt
hg clone https://rmbianchi@bitbucket.org/rmbianchi/soqt
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../soqt
make -j4
make install
cd ..

# Build GeoModelCore
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_core
cd build_core
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelCore
make -j2
make install
cd ..

# Build GeoModelIO
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_io
cd build_io
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelIO
make -j2
make install
cd ..

# Build GeoModelVisualization
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization.git
mkdir build_viz
cd build_viz
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelIO
make -j4
make install
cd ..
```
### Post install settings


#### Linux

```bash
# After compilation, you should apply this temporary fix:
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
```

#### macOS

```bash
# After compilation, you should apply two temporary fixes:
install_name_tool -add_rpath ../install/lib ../install/bin/gmex  # this is a temporary fix
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
```

### Run GeoModelExplorer (gmex)

Then, you can run your local copy of `gmex` with:

```bash
../install/bin/gmex
```

----

## Build the single libraries

### GeoModelCore

### GeoModelIO

### GeoModelG4

### GeoModelExamples
