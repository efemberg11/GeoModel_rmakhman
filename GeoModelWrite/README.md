
# Building the GeoModelIO::GeoModelWrite library

## Dependencies

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

#### CLHEP

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
### Build GeoModelIO dependencies

### Build GeoModelKernel

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

### Build GeoModelIO::GeoModelDBManager

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO/GeoModelDBManager.git
mkdir build_gmdb
cd build_gmdb
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelDBManager/
make -j 4
make install
```

### Build GeoModelIO::TFPersistification

```bash
git clone
mkdir TFPersistification_build
cd TFPersistification_build
cmake ../TFPersistification -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=../install
make -j10
make install
```

## Build GeoModelIO::GeoModelWrite

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO/GeoModelWrite.git
mkdir build_gmwrite
cd build_gmwrite
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelWrite
make -j 4
make install
```
