

# The 'hellogeoRead2G4' GeoModel example

The `hellogeoRead2G4` example shows you how to read persistified GeoModel data in a standalone program and convert it to a Geant4 geometry.

This is built on top of the [`hellogeo` example program](https://gitlab.cern.ch/GeoModelDev/GeoModelExamples/helloGeo).

## Build the dependencies

### Install Qt5

See: <https://doc.qt.io/qt-5/gettingstarted.html>

On macOS, you can use `brew`:

```bash
brew install qt5
```

After the installation, pay attention to add the Qt folder to your PATH (replace <path-to-qt> with your Qt installation folder and 'clang_64' with ):

```bash
PATHQT=/<path-to-qt>/Qt5.12.0/5.12.0/clang_64;
export PATH=$PATHQT/bin:$PATH;
```

On some platforms, you might need to explicitly set the QMAKESPEC and the QT variables:

```bash
export QMAKESPEC=$PATHQT/mkspecs/macx-clang;
export QTDIR=$PATHQT;
export QTINC=$PATHQT/include;
export QTLIB=$PATHQT/lib
```


### Build external dependencies

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

#### CLHEP

Geant4 needs [CLHEP](https://gitlab.cern.ch/CLHEP).

We checkout and build the latest release of CLHEP:

```bash
git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
cd CLHEP
git checkout CLHEP_2_4_1_0 # get the latest release
cd ../
mkdir CLHEP_build
cd CLHEP_build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../CLHEP/
make -j4
make install
```

### Build GeoModel dependencies

#### Build GeoModelCore

```bash
git clone --recurse-submodules ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelCore.git
mkdir build_gmk
cd build_gmk
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelCore
make -j 4
make install
```
#### Build GeoModelIO dependencies

```bash
git clone --recurse-submodules ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO.git
mkdir build_gmk
cd build_gmk
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelIO
make -j 4
make install
```
#### Build GeoModelG4 dependencies

```bash
git clone --recurse-submodules ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelG4.git
mkdir build_gmk
cd build_gmk
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelG4
make -j 4
make install
```


## Build 'hellogeo2g4'

From your work folder:

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelExamples/hellogeo.git
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo  ../GeoModelExamples/HelloGeoRead2G4
make -j4
make install
```
**Note:** If you installed Geant4 in a folder other than the `../install`, then you have to pass the path to Geant4 adding the -DGeant4_DIR option to cmake

```bash
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo  ../GeoModelG4/ -DGeant4_DIR=<path-to-Geant4-install>/lib/Geant4-10.x.y/
```

## Run `hellogeoRead2G4`

Get sample geometry data to play with:

```bash
wget https://atlas-vp1.web.cern.ch/atlas-vp1/doc_new/sample_datafiles/geometry/geometry-ATLAS-R2-2015-03-01-00.db
ln -s $PWD/geometry-ATLAS-R2-2015-03-01-00.db ../geometry.db
```

Now, you can run the example by typing:

```bash
./hellogeoRead2G4
```

The example program:

 1. loads the geometry from the `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
 6. transforms the GeoModel tree to Geant4 geometry
