# AGDD

this is the set of libraries for the ATLAS AGDD system, including the 
plugin (AGDDPlugin) needed to run it with gmex

## Cloning 

from e.g. $HOME:

git clone https://gitlab.cern.ch/GeoModelATLAS/agdd.git

## Building

from e.g. $HOME

mkdir build_agdd ; cd build_agdd
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Release ../agdd
make install

