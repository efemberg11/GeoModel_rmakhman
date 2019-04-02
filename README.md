# GeoModelG4

GeoModelG4 contains tools to interface GeoModel with the [Geant4](https://geant4.web.cern.ch) detector simulation toolkit.

```bash
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelG4/
make
make install
```

**NOTE:** If you experience issues with CMake not finding Geant4 properly, try to pass the Geant4 lib installation dir to CMake, to let it correctly find the Geant4Config.cmake file:

```bash
cmake -DGeant4_DIR=<path_to_geant4_install_dir>/lib/Geant4-10.5.0/ -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelG4/
```

Alternatively, you can source the Geant4 setup, before running cmake:

```bash
source <path_to_geant4_install_dir>/bin/geant4.sh
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelG4
```
