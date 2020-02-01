# GeoModelDataManagers

The package contains an implementation of the generic material manager for GeoModel. The material manager constructs transient element and material objects by reading their definitions from XML input files.

## Build GeoModelDataManagers package from source

Make sure you have built and installed the following packages:

- GeoModelCore (from GeoModelDev)
- GeoModelTools (from GeoModelDev)

After that proceed with building the package and installing it into ${INSTALL_DIR} as follows:

```bash
git clone https://gitlab.cern.ch/GeoModelATLAS/GeoModelDataManagers
mkdir build_datamanagers
cd build_datamanagers
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ../GeoModelDataManagers
make -j4
make instal
```

