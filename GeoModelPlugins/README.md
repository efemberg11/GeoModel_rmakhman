# GeoModelPlugins

This repository contains plugins that create GeoModel geometries.  To build these packages, follow the instructions in https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/-/blob/master/README.md. This README is about how to develop a plugin for ATLAS. 

The LAr and Tile systems are somewhat unique:  they make raw GeoModel calls.  They will not be addressed.  Instead we give instructions for developing plugins based upon GeoModelXML.  (https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/-/blob/master/GeoModelXML/README.md)


## Install the GeoModel tool suite.

The GeoModel tool suite includes the geometry explorer (gmex) and associated tools such as gmcat, gmclash, gm2gdml, gdml2gm, gmgeantino, and fullSimLight, all of which are useful in developing detector geometry.  You may build from source or, on some platforms, install the tools using native package managers. 

Installation instructions are at https://geomodel.web.cern.ch


## Install the XML-based, git-hosted detector description database. 

In Run 4, XML files containing detector description information will be located in subsystem-specific folders managed by git.  You may need to ask for  read and/or write permission to this git repository (boudreau@pitt.edu, or dellacqu@cern.ch, riccardo.maria.bianchi@cern.ch). 

Check out the repository:  https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelData. 
Set the environment variable GEOMODEL_XML_DIR to point to this directory. This tells the plugins where to look for their data. 
