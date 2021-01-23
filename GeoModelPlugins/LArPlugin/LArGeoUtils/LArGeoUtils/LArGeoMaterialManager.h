/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef LARGEOUTILS_LARGEOMATERIALMANAGER_H
#define LARGEOUTILS_LARGEOMATERIALMANAGER_H

#include <map>
#include <string>

class GeoMaterial;

class LArGeoMaterialManager
{
public:
  LArGeoMaterialManager();
  ~LArGeoMaterialManager();

  // Build LAr specific blended materials
  void buildMaterials();
  
  const GeoMaterial* getMaterial(const std::string& name) const;

private:
  std::map<std::string,GeoMaterial*> m_materials;
};

#endif
