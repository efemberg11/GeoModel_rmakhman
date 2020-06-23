# Geo2G4

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelG4Utils/GeoModel2G4.git
mkdir build_geo2g4
cd build_geo2g4
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModel2G4
make -j4
make install
```
