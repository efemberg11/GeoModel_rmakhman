# FullSimLight and FSL installation from source

FullSimLight and its graphical user interface FSL, are experiment agnostic detector simulation tools that have been implemented and are mantained by the [ATLAS](https://atlas.cern) collaboration. In what follows you will find instructions on how to build and install FullSimLight and FSL from the source code. If you are interested in testing FullSimLight and FSL with the ATLAS experiment configuration please refer to the ATLAS Extensions section below.

# Prerequisites

Before proceeding with this section please make sure that you visited the GeoModel installation instructions page to check the prerequisites list and install all the dependencies of GeoModel. 

FullSimLight and its tools depend on Geant4, Xerces-C and HDF5. Additionally, the user might want to install [Pythia8](http://home.thep.lu.se/Pythia/) and [HepMC3](https://gitlab.cern.ch/hepmc/HepMC3) as optional dependencies.

## Geant4 installation
There is no specific requirement for the version of Geant4 to use with FullSimLight. Our team tries to keep FullSimLight compatible with the most recent Geant4 versions. If you have any issues with a specific Geant4 version please open a ticket at the following [link](https://gitlab.cern.ch/GeoModelDev/GeoModel/-/issues). 

Before installing Geant4, check at the [Geant4 website](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html) the pre-requisites needed and install them.

!!! note

    Please note that an installation of Geant4 including the GDML extension (which requires the XercesC version >=3 package installed in the system) is required, i.e. the Geant4 toolkit must be built with the `-DGEANT4_USE_GDML=ON` CMake option.

###Xerces-C installation
Please choose the Xerces-C installation that is required by the Geant4 version that you are installing. As an example here we show how to install xerces-c 3.2.2.

```bash
wget https://archive.apache.org/dist/xerces/c/3/sources/xerces-c-3.2.2.tar.gz
tar -xf xerces-c-3.2.2.tar.gz
cd xerces-c-3.2.2 ; mkdir build  ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install ../
make -j8 ; make install
```

Now that you have installed xerces-c, you are ready to install Geant4. Clone the [Geant4 repository](https://gitlab.cern.ch/geant4/geant4.git), then:

```bash
git clone https://gitlab.cern.ch/geant4/geant4.git
cd geant4
git tag
```

Choose the release you want to use amongst the listed ones (i.e. Geant4-11.2.1), and checkout the corresponding tag:

```bash
git checkout tags/v11.2.1
```
We recommend to build Geant4 with the multithreading support on, so that you can take advantage of it and run your simulation in multithreaded mode.

!!! note

    To get the most performant Geant4 build, one needs to build the Geant4 toolkit
    with the following CMake options (in addition to -DGEANT4_USE_GDML=ON):

    ``` bash
    -DGEANT4_BUILD_MULTITHREADED=ON
    -DGEANT4_BUILD_VERBOSE_CODE=OFF
    -DGEANT4_BUILD_STORE_TRAJECTORY=OFF
    ```
    
```bash
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../  -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MULTITHREADED=ON
make -j8 ; make install
```


Before running FullSimLight and all the tools that depend on Geant4 please make sure to source the geant4.sh file to set correctly all the Geant4 environment variables.


##Pythia installation

If you wish to run FullSimLight with Pythia events you will need to install Pythia in your system. Follow the instructions at the [official website](http://home.thep.lu.se/Pythia/) for that. 
In order to build FullSimLight and FSL with Pythia you should then use the following flag: `-DGEOMODEL_USE_PYTHIA=ON` (see the following section for more details).

##HepMC3 installation

If you wish to run FullSimLight with HepMC3 events you will need to install HepMC3 in your system. Follow the instructions at the [official repository](https://gitlab.cern.ch/hepmc/HepMC3) for that. 
In order to build FullSimLight and FSL with HepMC3 you should then use the following flag: `-DGEOMODEL_USE_HEPCM3=ON`(see the following section for more details).


# How to build FullSimLight and FSL

FullSimLight and FSL can be built as part of the GeoModel build, by enabling the related configuration flags `GEOMODEL_BUILD_FULLSIMLIGHT` and `GEOMODEL_BUILD_FSL`. 

## Standard installation

If you want to install FullSimLight in the standard installation path you can follow the instructions below:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 ../GeoModel
make -j
make install
```

That will compile the base GeoModel packages (*i.e.*, `GeoModelCore`, `GeoModelIO`, and `GeoModelTools`), the GeoModel<->Geant4 interface package `GeoModelG4`, and the application `FullSimLight`.

This will install all the base libraries and the `fullSimLight` executable in the standard installation path (*e.g.*, `/usr/local` on macOS). If you want to have a local installation, see below.

!!! note

    If you are interested in running FullSimLight with the full ATLAS detector geometry and the real ATLAS magnetic field map, please refer to the ATLAS Extensions section below.


## Local installation

You can install FullSimLight in a local folder as well. This let you remove everything by simply deleting the local folder, and lets you have multiple versions of the packages installed.

You can install FullSimLight locally by using the `CMAKE_INSTALL_PREFIX` option. In the example below, we instruct CMake to install everything inside a local folder named `install` besides the `GeoModel` folder:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DCMAKE_INSTALL_PREFIX=../../install ../GeoModel
make -j
make install
```

At the end, you will find all the libraries and executables installed under:

```bash
ls ../../install
```

## Build FSL

FSL is the graphical user interface to FullSimLight. It allows you to configure your simulation and run it by using a json configuration file. Similarly to what said above, if you want to build FSL, you will have to enable the related configuration flag `GEOMODEL_BUILD_FSL`. If you enable the build of FSL automatically also FullSimLight will be built. 

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FSL=1 -DCMAKE_INSTALL_PREFIX=../../install ../GeoModel
make -j
make install
```

## How to build FullSimLight and FSL with Pythia

In order to build FullSimLight and FSL with Pythia you will need to activate the appropriate flag `GEOMODEL_USE_PYTHIA`:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FSL=1 -DGEOMODEL_USE_PYTHIA=ON -DCMAKE_INSTALL_PREFIX=../../install ../GeoModel
make -j
make install
```

If you installed Pythia in a local folder, CMake might not be able to find it. In this case you should specify manually the path to the include directory and to the library. 

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DGEOMODEL_USE_PYTHIA=ON - DPythia_INCLUDE_DIR=<path_to_your_Pythia_install_dir>/include -DPythia_LIBRARY=<path_to_your_Pythia_install_dir>/lib/<library name, i.e. libPythia.so/dylib> -DCMAKE_INSTALL_PREFIX=../../install ../GeoModel
make -j
make install
```

## How to build FullSimLight and FSL with HepMC3

In order to build FullSimLight and FSL with HepMC3 you will need to activate the appropriate flag:
```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DGEOMODEL_USE_HEPMC3=ON -DCMAKE_INSTALL_PREFIX=../../install ../GeoModel
make -j
make install
```

If you installed HepMC3 in a local folder, CMake might not be able to find it. In this case you should specify manually the path to the include directory and to the library. 

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DGEOMODEL_USE_HEPMC3=ON  -DHEPMC3_LIB=<path_to_your_HepMC3_install_dir>/lib/<library name, i.e. libHepMC3.so/dylib> -DCMAKE_INSTALL_PREFIX=../install ../GeoModel
make -j
make install
```

## How to use a custom version of Xerces-C

The Geant4 GDML format depends on the Xerces-C library. Therefore, different Geant4 releases can use different versions of the Xerces-C library.

If you want to build FullSimLight with a custom, locally installed Xerces-C library, you can pass the `XercesC_INCLUDE_DIR` and `XercesC_LIBRARY` variable to CMake while configuring the build of FullSimLight:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake  -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DXercesC_INCLUDE_DIR=<path-to-local-XercesC-installation>/include -DXercesC_LIBRARY=<path-to-local-XercesC-installation>/lib/libxerces-c.dylib ../GeoModel/
```

!!! note
    
    When setting the path of the local installation of Xerces-C, please be sure to use a full path or a local path related to the `$PWD` environment variable For example, those are valid examples:

    ```bash
    -DXercesC_INCLUDE_DIR=/opt/myXercesC/include    # using a full 'full path' 
    -DXercesC_INCLUDE_DIR=$PWD/../myXercesC/include # using a 'relative path', relative to the current build folder 
    ```

!!! note

    If you installed Geant4 on myour machine with a package manager (*e.g.*, Homebrew on macOS) the version used by the Geant4 package built by the package manager could be hardcoded in the package configuration itself. That could cause the output of warning messages by CMake, of which thev one below is an example:

    ```bash
    Value of 'XercesC_INCLUDE_DIR' is already set and does not match value set at Geant4 build-time
    ```

    Normally, those warnings do not harm the build. But please judge by yourself if the difference isn the Xerces-C library can affect the project you are working on.

## Combining different configuration/build options

You can also combine different configuration options. For example, here below we build FullsimLight by using the built-in `nlohmann-json` library together with the custom Xerces-C library, and we install the outcome in a local `../install` folder: 

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
cd GeoModel
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DGEOMODEL_USE_BUILTIN_JSON=1  -DGEOMODEL_BUILD_FULLSIMLIGHT=1 -DXercesC_INCLUDE_DIR=<path-to-local-XercesC-installation>/include -DXercesC_LIBRARY=<path-to-local-XercesC-installation>/lib/libxerces-c.dylib ../GeoModel/
```


# How to build the ATLAS Extensions submodule

FullSimLight is an experiment agnostic detector simulation tool that has been implemented and is mantained by the [ATLAS](https://atlas.cern) collaboration. Its initial goal was to make it possible to simulate the full ATLAS detector with the real magnetic field map in full standalone mode. All the ATLAS specific customizations are available at the [ATLAS Extensions](https://gitlab.cern.ch/atlas/geomodelatlas/ATLASExtensions) repository that is linked to the GeoModel repository as a git submodule. In order to have the corresponding code cloned inside the GeoModel repository the user would need to clone recursively the main repository. 

If you didn't clone the GeoModel repository, you can clone it recursively:

```c++
git clone --recursive https://gitlab.cern.ch/GeoModelDev/GeoModel.git
```

Alternatively, if you already cloned the GeoModel repository, it will be sufficient to type the following commands from the main `GeoModel` repository folder:

```c++
cd GeoModel
git submodule init
git submodule update
```

A flag **GEOMODEL_BUILD_ATLASEXTENSIONS** is available to build the ATLAS extentions (by default OFF) and it should be turned ON in order to build all the extensions. If you want to build the ATLAS Extensions and FullSimLight you should then:

```c++
cd GeoModel
mkdir build ; cd build
cmake ../ -DGEOMODEL_BUILD_ATLASEXTENSIONS=ON -DGEOMODEL_BUILD_FULLSIMLIGHT=ON
make -j
make install
```

A configuration file **atlas-conf.json** with all the ATLAS specific customizations is provided with the ATLASExtensions and will be installed under **<install>/share/FullSimLight/ATLAS**. In order to run a Geant4 simulation with the ATLAS configuration please visit the [ATLAS Extensions page](https://geomodel.web.cern.ch/home/fullsimlight/atlas-extensions/). 




