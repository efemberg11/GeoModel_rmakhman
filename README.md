# GeoModelPlugins

This repository contains plugins that create GeoModel geometries.  They are dual use in the sense that they function completely outside of Athena, but also are able to function within Athena. 

# Build from source.

To build  plugins for the GeometryExplorer <https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization>:
 
* Install the GeoModelCore development kit (quick method):

      sudo add-apt-repository ppa:kaktusjoe/geomodel

      sudo apt-get update

      sudo apt install geomodel-core-dev

* Install the GeoModelCore development kit (from source):

      follow instructions at <https://gitlab.cern.ch/GeoModelDev/GeoModelCore>

* Build and install the plugin(s)
    
      cd GeoModelPlugins

      mkdir build

      cd build 

      cmake ..

      make -j

      sudo make install
      
* Your plugin is ready!  Run it with gmex (see <https://gitlab.cern.ch/GeoModelDev/GeoModelVisualization> )