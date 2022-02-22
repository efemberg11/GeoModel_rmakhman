/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOG4SOLIDPLUGINLOADER_H_
#define GEOG4SOLIDPLUGINLOADER_H_
#include <string> 
class GeoVG4SolidPlugin;
class GeoG4SolidPluginLoader 
{
 public:

  // Constructor:
  GeoG4SolidPluginLoader();

  // Destructor:
  ~GeoG4SolidPluginLoader();

  // load G4Solid plugin
  GeoVG4SolidPlugin *load(const std::string & path) const;
  
 private:
  
  GeoG4SolidPluginLoader(const GeoG4SolidPluginLoader &)=delete;
  GeoG4SolidPluginLoader & operator=(const GeoG4SolidPluginLoader &)=delete;

};

#endif
