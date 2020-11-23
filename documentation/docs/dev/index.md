# Build the GeoModel libraries

Here below, you will find instructions on how to compile the GeoModel libraries from scratch. You can want to do that for different reasons:

- you want to build the libraries for an OS which is not officially supported or for a version of it for which no pre-compiled packages are provided (for info, see the [Start](../start/) section)
- you want to test the latest version of the code
- you want to contribute to the development of the libraries

In the following, you will find basic strategies to effectively build the GeoModel libraries, based on different use cases. For more details, you can find updated and additional instructions inside the `README` file of the [GeoModelVisualization/README.md](https://gitlab.cern.ch/GeoModelDev/geomodelvisualization/-/blob/master/README.md) specific package.

## System Dependencies

### Compilers

In the GeoModel packages, we use modern C++ constructs (C++ 14 and sometimes C++17). Thus, before trying to compile the GeoModel libraries, be sure your compiler supports C++17.

!!! note

    If you use GCC, we successfully tested the build of GeoModel packages on GCC 6.2 and beyond (GCC 7, GCC 8).


### macOS

```bash
# install external dependencies
brew install cmake eigen doxygen wget boost xerces-c nlohmann-json sqlite

# if you want to build the visualization tools as well, please install and configure Qt5
brew install qt5
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.zshrc
```

Before building the GeoModel libraries from source, as explained here below, please check if you have old versions of them installed on your system and remove them.
Also, you should remove all GeoModel packages previously installed through `brew`, because they could be picked up by the build system while building the sources, affecting the compilation.

You can check if you have GeoModel libraries which had installed with Homebrew by typing:

```
brew list
```

Then, you can remove the old GeoModel packages with, for example:

```
brew remove geomodelcore
```

Please remove **all** the `geomodel`-packages before trying to build the packages from source.

See also the [Troubleshooting](troubleshooting.md) page for additional help.


### Linux/Ubuntu

```bash
sudo apt-get update -qq && sudo apt-get install -y -qq git cmake wget unzip build-essential freeglut3-dev libboost-all-dev qt5-default libeigen3-dev libxerces-c-dev
```

### Linux/Fedora

```bash
dnf install --assumeyes make automake gcc gcc-c++ cmake git qt5  boost mercurial xerces-c-devel unzip freeglut-devel wget eigen3-devel
```

### Centos/RedHat



## Quick instructions - Build everything

With these instructions you will build the whole the software stack for GeoModel development. The GeoModel libraries will be built from the HEAD version of the 'master' branch. If something does not compile, please [let the developers' team know](../contacts.md).


### Build and Install locally

These instructions will install the libraries and the tools in a local `install` folder. That is useful for developmemnt, because yuo can handle multiple versions installed on the same system.

However, if you prefer to install the tools and the libraries in the `/usrl/local` system directory, just remove from the commands the option `-DCMAKE_INSTALL_PREFIX=../install`.

#### Build the base libraries and tools (GeoModelCore, GeoModelIO, GeoModelTools)

This will install the base libraries and tools, the ones conatined in the packages GeoModelCore (GeoModelKernel), GeoModelIO (I/O libraries and the interface to the SQLite engine), and GeoModelTools (XML and JSON parsers, GeoModelXML, the `gmcat` concatenation command line tool):

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
mkdir build_geomodel
cd build_geomodel
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModel
make -j
make install
cd ..
```

A note: on some platforms, the default Eigen, Xerces-C, and nlohmann_json libraries installed through the platform's package manager are quite old. If you desire, you can build GeoModel with a builtin version of those libraries by enabling the corresponding options at configuration time:

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_USE_BUILTIN_EIGEN3=1 -DGEOMODEL_USE_BUILTIN_XERCESC=1 -DGEOMODEL_USE_BUILTIN_JSON=1 ../GeoModel
```

If you compile on Centos7, that above could be a convenient option.


#### Build the optional packages 

There are several options offered to the user to selectively build the optional packages. 

!!! note
    You can see all the available options, and you can enable/disable them inetractively, by using the command for the interactive CMake configuration `ccmake`, instead of the classical `cmake`; *i.e.*: `cmake ../GeoModel`.


#### Build the visualization tools (GeoModelVisualization)

##### 3D graphics dependencies - macOS

On macOS, you can install the needed graphics dependencies with `brew`:

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install simage coin-bb soqt-bb
```


##### 3D graphics dependencies - macOS

On Linux, you can build the needed graphics libraries by following the instructions below.


###### a) Simage

On all platforms *except Centos7*, you can build Simage by followoing these instructions:

```bash
# Build Simage
wget http://cern.ch/atlas-software-dist-eos/externals/Simage/Coin3D-simage-2c958a61ea8b.zip
unzip Coin3D-simage-2c958a61ea8b.zip
cd Coin3D-simage-2c958a61ea8b
./configure --prefix=$PWD/../install
make -j
make install 
cd ..
```

*On Centos7*, you have to apply a patch to build the package. Therefore, on Centos7 please build Simage by following the instructions below:

```bash
# Build Simage
wget http://cern.ch/atlas-software-dist-eos/externals/Simage/Coin3D-simage-2c958a61ea8b.zip
unzip Coin3D-simage-2c958a61ea8b.zip
cd Coin3D-simage-2c958a61ea8b
./configure --prefix=$PWD/../install
wget -O cc7.patch https://gitlab.cern.ch/atlas/atlasexternals/-/raw/master/External/Simage/patches/libpng_centos7.patch?inline=false 
patch -p1 < cc7.patch
make -j
make install 
cd ..
```


###### b) Coin3D & SoQt

Now, you should build Coin3D (the 3D graphics engine) and SoQt (the glue package between the 3D graphics engine, Coin, and the windowing system, Qt5):

```bash
# Build Coin3D
wget https://atlas-vp1.web.cern.ch/atlas-vp1/sources/coin-4.0.0-src.zip
unzip coin-4.0.0-src.zip -d coin-sources
mv coin-sources/* coin
mkdir build_coin
cd build_coin
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DCOIN_BUILD_TESTS=0 ../coin
make -j
make install
cd ..

# Build SoQt
wget -O soqt.zip http://cern.ch/atlas-software-dist-eos/externals/SoQt/soqt_ea5cd76.zip
unzip soqt.zip
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DSOQT_BUILD_DOCUMENTATION=0 ../soqt
make -j
make install
cd ..
```

##### Build GeoModelVisualization (a.k.a, `gmex`)

Now, you can build the GeoModel visualization tools by enabling the corresponding option in the CMake configuration of GeoModel.

To do that, come back to the `build_geomodel` folder we created earlier, or create a new directory for this build right now, then run:

```
cd build_geomodel # or 'cd' to the new directory you have just created for this specific build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_BUILD_VISUALIZATION=1 ../GeoModel
make -j
make install
cd ..
```


### (Optional) Build an example GeoModelPlugin



```bash
# Build the GeoModelTools
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelTools.git
mkdir build_tools
cd build_tools
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelTools
make -j
make install
cd ..

# Build the GeoModelATLAS/GeoModelDataManagers
git clone https://gitlab.cern.ch/GeoModelATLAS/GeoModelDataManagers.git
mkdir build_managers
cd build_managers
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelDataManagers
make -j
make install
cd ..

# Build atlas/GeoModelPlugins
git clone https://gitlab.cern.ch/atlas/GeoModelPlugins.git
mkdir build_plugins
cd build_plugins
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../GeoModelPlugins
make -j
make install
cd ..
```


### Post install settings

Assuming you followed the above instructions and you used `../install` as the installation path.

#### macOS

```bash
# After compilation, you should apply two temporary fixes:
install_name_tool -add_rpath ../install/lib ../install/bin/gmex  # This is a temporary fix. NOTE: in case you are developing gmex code, this should be run every time you compile and install a new version of gmex.
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
```

#### Linux/Ubuntu

```bash
# After compilation, you should apply this temporary fix:
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
export LD_LIBRARY_PATH=${PWD}/../install/lib/ # this is a temporary fix
```

#### Linux/Fedora

```bash
# After compilation, you should apply this temporary fix:
export GXPLUGINPATH=../install/lib/gxplugins # this is a temporary fix
export LD_LIBRARY_PATH=../install/lib:../install/lib64:$LD_LIBRARY_PATH # this is a temporary fix, we will fix the installation on Fedora
```



### Run GeoModelExplorer (gmex)

Then, you can run your local copy of `gmex` with:

```bash
../install/bin/gmex
```

----

## Troubleshooting

For a collection of suggestions on how to fix potential errors and glitches, please refer to the dedicated [Troubleshooting](troubleshooting.md) page.


