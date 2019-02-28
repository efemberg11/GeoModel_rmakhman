
# GeoMaterial2G4 build

## Dependencies

### Build external dependencies

#### CLEHP

We checkout and build the latest release of CLHEP:

```bash
git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
cd CLHEP
git checkout CLHEP_2_4_1_0
cd ../
mkdir build_clhep
cd build_clhep
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../CLHEP/
make -j 4
make install
```

#### Geant4

We checkout and build the latest release of Geant4:

```bash
git clone https://gitlab.cern.ch/geant4/geant4.git
cd geant4
git checkout v10.5.0
cd ../
mkdir build_geant4
cd build_geant4
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../geant4/
make -j 4
make install
```

### Build GeoModel dependencies

#### GeoModelKernel

**NOTE!** Here, we are currently using a development branch.

```bash
git clone --recurse-submodules ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelKernel.git
cd GeoModelKernel
git checkout master-geomodel-standalone-cmake
cd ../
mkdir build_gmk
cd build_gmk
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelKernel
make -j 4
make install
```

## Build GeoMaterial2G4

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelG4Utils/GeoMaterial2G4.git
mkdir build_gmat2g4
cd build_gmat2g4
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoMaterial2G4
make -j4
make install
```

**NOTE:** If you experience issues with CMake, try to pass the Geant4 lib installation dir to CMake, to let it correctly find the Geant4Config.cmake file:

```bash
cmake -DGeant4_DIR=../install/lib/Geant4-10.5.0/ -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoMaterial2G4/
```
