cd build

cmake ../GeoModel -DCMAKE_INSTALL_PREFIX=../install

make -j20 install

export LD_LIBRARY_PATH=/home/rmakhman/geant4/Geant4-install/lib:/usr/lib/x86_64-linux-gnu:/home/rmakhman/dev_geomodel/install/lib


