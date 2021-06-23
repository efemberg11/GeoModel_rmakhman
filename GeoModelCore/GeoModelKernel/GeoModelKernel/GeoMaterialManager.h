/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMATMANAGER_GEOMATMANAGER_H
#define GEOMATMANAGER_GEOMATMANAGER_H

#include <iostream>

class GeoElement;
class GeoMaterial;
class Impl;

class GeoMaterialManager {
  
  friend class Impl;
  
 public:
  static const GeoMaterialManager* getManager();
  
  const GeoMaterial *getMaterial(const std::string & name) const;
  const GeoElement *getElement(const std::string & name) const;
  const GeoElement *getElement(unsigned int atomicNumber) const;

  std::ostream & printAll(std::ostream & o=std::cout) const;

 protected:
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
  static GeoMaterialManager* s_instance;

  GeoMaterialManager();
  ~GeoMaterialManager();

  // Build special materials
  // void buildSpecialMaterials();
};

#endif
