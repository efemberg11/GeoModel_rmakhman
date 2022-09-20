# LArCustomSolid Extension

This repository contains the LArCustomSolid Extension that describe the ATLAS LAr EMEC custom shape. The LAr EMEC custom shape extension is build starting from a copy of the corresponding code in the [Athena](https://gitlab.cern.ch/atlas/athena) repository. 

## How to build the LArCustomSolid Extension
The LArCustomSolid Extension depends on GeoModel, so you need to install first GeoModel from this [repository](https://gitlab.cern.ch/GeoModelDev/GeoModel). 

```bash
git clone https://gitlab.cern.ch/SolidExtensions/LArCustomSolidExtension.git
cd LArCustomSolidExtension
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=<path_to_GeoModel_install_dir>
make -j
make install
```
This will produce a LArCustomSolidExtension.so/dylib library that can be used to build the EMEC detector.

## How to update the LArCustomSolid Extension

To keep the code of the LArCustomSolid Extention synchronized with the latest available version in the [Athena](https://gitlab.cern.ch/atlas/athena) repository, just run the autonomous-lar.sh script.
