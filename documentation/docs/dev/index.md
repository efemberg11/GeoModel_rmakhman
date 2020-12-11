# Build the GeoModel libraries

Here below, you will find instructions on how to compile all the GeoModel libraries and tools, and their main dependencies, from sources. You may want to do that for different reasons:

- you want to build the libraries for an OS which is not officially supported or for a version of it for which no pre-compiled packages are provided (for info, see the [Start](../start/install.md) section)
- you want to test the latest version of the code
- you want to contribute to the development of the GeoModel libraries and tools

In the following, you will find basic strategies to effectively build the GeoModel software stack, based on different use cases.


## System Dependencies

### Compilers

In the GeoModel packages, we use **modern C++** constructs (C++ 14 and sometimes C++17). Thus, before trying to compile the GeoModel libraries, be sure your compiler supports **C++17**.

!!! note

    If you use GCC, we successfully tested the build of GeoModel packages on GCC 6.2 and beyond (GCC 7, GCC 8, GCC 9). 

    If you use Clang (mainly on macOS), we successfully tested the GeoModel build on Clang/Xcode 10 and Clang/Xcode 11.


### Third-party libraries - macOS

On macOS, you can install all the needed third-party dependencies by using the [Homebrew](https://brew.sh) package manager:

```bash
# install external dependencies
brew install cmake eigen doxygen wget boost xerces-c nlohmann-json sqlite

# if you want to build the visualization tools as well, please install and configure Qt5
brew install qt5
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.zshrc
```

!!! warning

    Before building the GeoModel libraries from source, please check if you have old versions of them installed on your system and remove them.
    Also, you should remove all GeoModel packages previously installed through `brew`, because they could be picked up by the build system while building the sources, affecting the compilation.
    
    Please remove **all** the `geomodel`-packages before trying to build the packages from source.

    See the [Troubleshooting](troubleshooting.md) page for additional details and instructions.



### Linux/Ubuntu

On Ubuntu, you can install all the needed third-party dependencies by using the built-in `apt` package manager:

```bash
sudo apt-get update -qq && sudo apt-get install -y -qq git cmake wget unzip build-essential freeglut3-dev libboost-all-dev qt5-default libeigen3-dev libxerces-c-dev libsqlite3-dev
```

### Linux/Fedora

On Fedora, you can install all the needed third-party dependencies by using the built-in `dnf` package manager:

```bash
dnf install --assumeyes make automake gcc gcc-c++ cmake git qt5  boost mercurial xerces-c-devel unzip freeglut-devel wget eigen3-devel
```

### Centos/RedHat

*coming soon*


## Visualization 3D graphics dependencies

### 3D graphics dependencies - macOS

On macOS, you can install the needed graphics dependencies with `brew`.

If you have not installed the `atlas/geomodel` Tap already, please install it now, by running the command below; otherwise, jspi this and jump to the next point.

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git 
```

Now, update your `atlas/geomodel` Tap to the latest version and install the graphics libraries:


```
brew update
brew install soqt-geomodel  
```

The second command will install all the latest graphics libraries needed by the visualization tools of GeoModel:  `simage-geomodel`, `coin-geomodel`, and `soqt-geomodel`.


### 3D graphics dependencies - Linux

On Linux, you can build the needed graphics libraries by following the instructions below.


#### a) Simage

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


#### b) Coin3D & SoQt

!!! warning

    The Coin sources are not compatible with CMake 3.19 (see https://gitlab.cern.ch/GeoModelDev/GeoModel/-/issues/7). Therefore, for the moment, until the Coin sources will be ported to CMake 3.19, you should use CMake <= 3.18.X to compile Coin. 

    Please note that, at the time of writing, Homebrew, the package manager for macOS, has updated its version of CMake from 3.18.4 to 3.19. Thus, you cannot build Coin on macOS with the CMake version installed by the `brew` command.

    You can install CMake on your system by downloading the installer from the [CMake website](https://cmake.org/download/).

    Or, you could have still  an older version on your system, which you can use. On macOS, and if those were installed with `brew`, you can check which versions of CMake you have in the path `/usr/local/Cellar/cmake/`. Then, you can use one of the ones you have installed by running its `cmake` binary command directly; for example: `/usr/local/Cellar/cmake/3.18.4/bin/cmake ...` .



Now, you should build Coin3D (the 3D graphics engine) and SoQt (the glue package between the 3D graphics engine, Coin, and the windowing system, Qt5):


```bash
# Build Coin3D
wget -O coin.zip https://atlas-vp1.web.cern.ch/atlas-vp1/sources/coin_c8a8003d4_1Dec2020.zip
unzip coin.zip
mkdir build_coin
cd build_coin
# NOTE: replace the path below with the patch of your CMake <= 3.18.X installation
path-to-cmake-3.18/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DCOIN_BUILD_TESTS=0 -DCMAKE_CXX_FLAGS=-Wno-deprecated-declarations ../coin
make -j
make install
cd ..

# Build SoQt
wget -O soqt.zip http://cern.ch/atlas-software-dist-eos/externals/SoQt/soqt_5796270_1Dec2020.zip
unzip soqt.zip
mkdir build_soqt
cd build_soqt
path-to-cmake-3.18/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DSOQT_BUILD_DOCUMENTATION=0 -DCMAKE_CXX_FLAGS=-Wno-deprecated-declarations ../soqt
make -j
make install
cd ..
```


## Quick instructions - Build everything

With these instructions you will build the whole the whole software stack for GeoModel development. The GeoModel libraries will be built from the HEAD version of the 'master' branch. If something does not compile, please [let the developers' team know](../about/contacts.md). 

With these instructions, you will build: `GeoModelCore`, `GeoModelIO`, `GeoModelTools`, `GeoModelVisualization` (a.k.a., `gmex`).


!!! note

    These instructions will install the libraries and the tools in a **local** `install` folder. That is very useful for developmemnt, because yuo can handle multiple versions installed on the same system.

    However, if you prefer to install the tools and the libraries in the `/usrl/local` system directory, just remove from the commands the option `-DCMAKE_INSTALL_PREFIX=../install`.



This will install everything: the base libraries contained in the packages `GeoModelCore` (the kernel libraries and utilities) and `GeoModelIO` (the I/O libraries and the interface to the underlying data formats and/or databases), the tools contained in `GeoModelTools` (XML and JSON parsers, the `GeoModelXML` interface, the `gmcat` concatenation command line tool), and the viusalization tools contained in `GeoModelVisualization` (`gmex`):


```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
mkdir build_geomodel
cd build_geomodel
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_BUILD_TOOLS=1 -DGEOMODEL_BUILD_VISUALIZATION=1 ../GeoModel
make -j
make install
cd ..
```

!!! note

    On some platforms (for example, on Centos 7 and some Ubuntu releases), the default `Eigen`, `Xerces-C`, and `nlohmann-json` libraries installed through the platform's package manager are quite old. If you desire, you can build GeoModel with a built-in, tested version of those libraries by enabling the corresponding options at configuration time with these additional build options:

    ```
    -DGEOMODEL_USE_BUILTIN_EIGEN3=1 -DGEOMODEL_USE_BUILTIN_XERCESC=1 -DGEOMODEL_USE_BUILTIN_JSON=1
    ```

    If you compile on Centos7, that above could be a very convenient option, which lets you skip the installation of custom versions of those libraries directly on your system. 

    A full example on Centos 7 could be:

    ```
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_BUILD_TOOLS=1 -DGEOMODEL_BUILD_VISUALIZATION=1 -DGEOMODEL_USE_BUILTIN_EIGEN3=1 -DGEOMODEL_USE_BUILTIN_XERCESC=1 -DGEOMODEL_USE_BUILTIN_JSON=1  ../GeoModel
    ```

!!! note

    In these instructions we are building everything, because that is the most used configuration. But the GeoModel build is highly modular, and you can install different combinations of the GeoModel subpackages. 

    You can see all the available options, and you can enable/disable them inetractively, by using the command for the interactive CMake configuration `ccmake`, instead of the classical `cmake`; *i.e.*: `cmake ../GeoModel`.

    *More details on that coming soon....*





## (Optional) Build an example GeoModelPlugin



```bash
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


## Post install settings

Assuming you followed the above instructions and you used `../install` as the installation path, in order to run the applications you need to set a couple of system variables.

!!! note

    You don't need to set the variables below, or run any system tool, if you installed GeoModel binaries by following the instructions on the [Install](../start/install.md) page.

    Also, yuo don't need them if you have omitted the local installation option (*i.e.*, the `-DCMAKE_INSTALL_PREFIX=../install` build option) while building and, therefore, you have installed everything i to system folders (like, for example, into `/usr/local/`).


### macOS

```bash
install_name_tool -add_rpath ../install/lib ../install/bin/gmex  
```

**Note:** You need to run the command above each time you install a new `gmex` executable while developing; *i.e.*, every time you run `make install`, if you are developing `GeoModelVisualization`. That is needed because a new `gmex` executable does not have the `rpath` set, and you must set it before running it, otherwise the executable cannot find the libraries it is linked against.

This will be automated in a future version.


### Linux/Ubuntu

```bash
export LD_LIBRARY_PATH=${PWD}/../install/lib/ # this is a temporary fix
```

### Linux/Fedora

```bash
export LD_LIBRARY_PATH=../install/lib:../install/lib64:$LD_LIBRARY_PATH # this is a temporary fix, we will fix the installation on Fedora
```



## Run GeoModelExplorer (gmex)


Then, you can download an example geometry data file:

{!start/get-geometry-example-file.md!}

and run your local copy of the GeoModel visualization tool, `gmex`, with (assuming you are still in the build folder):

```bash
../install/bin/gmex
```

----

## Troubleshooting

For a collection of suggestions on how to fix potential errors and glitches, please refer to the dedicated [Troubleshooting](troubleshooting.md) page.


