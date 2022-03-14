/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOG4EXTENSIONSOLIDLOADER_H_
#define GEOG4EXTENSIONSOLIDLOADER_H_
#include <string> 
class GeoVG4ExtensionSolid;
class GeoG4ExtensionSolidLoader 
{
 public:

  // Constructor:
  GeoG4ExtensionSolidLoader();

  // Destructor:
  ~GeoG4ExtensionSolidLoader();

  // load G4Solid plugin
  GeoVG4ExtensionSolid *load(const std::string & path) const;
  
 private:
  
  GeoG4ExtensionSolidLoader(const GeoG4ExtensionSolidLoader &)=delete;
  GeoG4ExtensionSolidLoader & operator=(const GeoG4ExtensionSolidLoader &)=delete;

};

#endif
