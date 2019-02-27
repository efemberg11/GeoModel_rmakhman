# GeoModelIO::TFPersistification build

## Build external dependencies

### Eigen

```bash
brew install eigen
```

### CLHEP

We checkout and build the latest release of CLHEP:

```bash
git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
cd CLHEP
git checkout CLHEP_2_4_1_0
cd ../
mkdir CLHEP_build
cd CLHEP_build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../CLHEP/
make -j4
make install
```

## Build GeoModelIO::TFPersistification

```bash
git clone
mkdir TFPersistification_build
cd TFPersistification_build
cmake ../TFPersistification -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=../install
make -j10
make install
```
