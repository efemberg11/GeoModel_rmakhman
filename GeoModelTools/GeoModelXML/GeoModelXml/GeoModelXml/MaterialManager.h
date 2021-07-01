/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEO_MODEL_XML_MATERIAL_MANAGER_H
#define GEO_MODEL_XML_MATERIAL_MANAGER_H

#include "GeoModelKernel/IGeoMaterialManager.h"

#include <iostream>

class GeoElement;
class GeoMaterial;
class Impl;

class MaterialManager: public IGeoMaterialManager {
  
  friend class Impl;
  friend class Gmx2Geo;
  
 public:
  //static const MaterialManager* getManager();
  
  const GeoMaterial *getMaterial(const std::string & name) const;
  const GeoElement *getElement(const std::string & name) const;
  const GeoElement *getElement(unsigned int atomicNumber) const;

  std::ostream & printAll(std::ostream & o=std::cout) const;

  void addElement(const std::string &name
		  , const std::string &symbol
		  , double z
		  , double a);

  void addNamespace(const std::string &name);

  void addMaterial(const std::string &name
		   , double density);

  void addMatComponent(const std::string &name
		       , double fraction);

  void lockMaterial();

  void dump();

 private:
  Impl* m_pImpl;
  //static MaterialManager* s_instance;

  MaterialManager();
  ~MaterialManager();

  // Build special materials
  // void buildSpecialMaterials();
};

#endif
