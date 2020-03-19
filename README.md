# GeoModelG4

GeoModelG4 contains tools to interface GeoModel with the [Geant4](https://geant4.web.cern.ch) detector simulation toolkit.

# Installation

## Dependencies:

## Geant4:

Before installing Geant4, check at the Geant4 website the pre-requisites needed:
http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/gettingstarted.html
Note in particular that to enable the use of geometry reading/writing from GDML XML files, the Xerces-C++ headers and library >=3 must be installed.
Clone the repository at this [address](https://gitlab.cern.ch/geant4/geant4.git), then:

```bash
git clone https://gitlab.cern.ch/geant4/geant4.git
cd geant4
git tag
```
Choose the release you want to use, i.e. Geant4-10.6.0, and checkout the corresponding tag:

```bash
git checkout tags/v10.6.0
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../  -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -GEANT4_BUILD_MULTITHREADED=ON
make
make install
```

## GeoModelCore:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelCore), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
cd GeoModelCore
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```
## GeoModelIO:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelIO).

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
cd GeoModelIO
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```

## GeoModelG4:

Clone the repository at this [address](https://gitlab.cern.ch/GeoModelDev/GeoModelG4), then:

```bash
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelG4.git
cd GeoModelG4
mkdir build ; cd build
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=Release ../
make
make install
```

**NOTE:** If you experience issues with CMake not finding Geant4 properly, try to pass the Geant4 lib installation dir to CMake, to let it correctly find the Geant4Config.cmake file:

```bash
cmake -DGeant4_DIR=<path_to_geant4_install_dir>/lib/Geant4-10.5.0/ -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../
```

Alternatively, you can source the Geant4 setup, before running cmake:

```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
cmake -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../
```
