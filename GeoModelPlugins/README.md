# GeoModelPlugins

This repository contains plugins that create GeoModel geometries.  To build these packages, follow the instructions in https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/-/blob/master/README.md. This README is about how to develop a plugin for ATLAS. 

The LAr and Tile systems are somewhat unique:  they make raw GeoModel calls.  They will not be addressed.  Instead we give instructions for developing plugins based upon GeoModelXML.  (https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelATLAS/-/blob/master/GeoModelXML/README.md)


## Install the GeoModel tool suite

The GeoModel tool suite includes the geometry explorer (gmex) and associated tools such as gmcat, gmclash, gm2gdml, gdml2gm, gmgeantino, and fullSimLight, all of which are useful in developing detector geometry.  You may build from source or, on some platforms, install the tools using native package managers. 

Installation instructions are at https://geomodel.web.cern.ch


## Install the XML-based, git-hosted detector description database

In Run 4, XML files containing detector description information will be located in subsystem-specific folders managed by git.  You may need to ask for  read and/or write permission to this git repository (boudreau@pitt.edu, or dellacqu@cern.ch, riccardo.maria.bianchi@cern.ch). 

Check out the repository:  https://gitlab.cern.ch/atlas/geomodelatlas/GeoModelData. 
Set the environment variable GEOMODEL_XML_DIR to point to this directory. This tells the plugins where to look for their data. 

## Add your XML files to your local copy

The directory structure is not yet extensive and is experimental.  The idea is to partition it by subystem, with further levels of hierarchy for subsubsystem and so forth. This is up to you to decide.  

## Make your plugin aware of the files in the local copy of the database

The embryonic muon system plugin provides an easy example to follow.  It appears in its entirety here: 


    #define  PLUGINNAME MuonPlugin

    #include "PluginTemplate.h"

    const std::vector<std::string> MuonPlugin::inputFiles={"Muons/Muons00.xml"};
    extern "C" MuonPlugin *createMuonPlugin() {
      return new MuonPlugin;
    }

Note, because of the plugin mechanism:

* The name of the class matches the PLUGINNAME macro definition
* The name of the source code file (MuonPlugin.cxx) matches the name of the class. 
* The name and signature of the creation method (extern "C" MuonPlugin* createMuonPlugin() ) matches the name of the class. 

If you wish to add an additional plugin, these conventions must be respected in order that the plugin properly loads into gmex and other tools. 

The plugin code includes a list of files to be accessed, that is relative to the directory $GEOMODEL_XML_DIR.  In the above example, that is only one file.  But, you can add as many as you like.  They can be added in the initializer, or they can be added programmatically in the creation method.  

That's all! You can now load your plugin into gmex. (Don't forget to set the GEOMODEL_XML_DIR to your local repository as described above.) For example

on linux:

```
gmex /path/to/libMuonPlugin.so
```

on mac: 

```
gmex /path/to/libMuonPlugin.dylib
```

In addition, you may now use any of the other tools in the tool suite (gmclash, gmgeantino, gmcat, fullSimLight...)
