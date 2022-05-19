/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef FSLSDPLUGINLOADER_H_
#define FSLSDPLUGINLOADER_H_
#include <string> 
class FSLSensitiveDetectorPlugin;
class FSLSDPluginLoader 
{
 public:

  // Constructor:
  FSLSDPluginLoader();

  // Destructor:
  ~FSLSDPluginLoader();

  // load G4Solid plugin
  FSLSensitiveDetectorPlugin  *load(const std::string & path) const;
  
 private:
  
  FSLSDPluginLoader(const FSLSDPluginLoader &)=delete;
  FSLSDPluginLoader & operator=(const FSLSDPluginLoader &)=delete;

};

#endif
