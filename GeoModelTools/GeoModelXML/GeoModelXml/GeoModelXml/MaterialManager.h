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

  const GeoMaterial *getMaterial(const std::string & name) const;
  const GeoElement *getElement(const std::string & name) const;
  const GeoElement *getElement(unsigned int atomicNumber) const;

  void printAll() const;

  void addElement(const std::string &name
		  , const std::string &symbol
		  , double z
		  , double a);

  void addElement(GeoElement*);

  void addNamespace(const std::string &name);

  void addMaterial(const std::string &name
		   , double density);

  void addMaterial(GeoMaterial*);

  void addMatComponent(const std::string &name
		       , double fraction);

  void lockMaterial();

  void dump();

  bool isMaterialDefined(const std::string s) const;

  bool isElementDefined(const std::string s) const;

  static MaterialManager* getManager();
 private:
  Impl* m_pImpl;
  static MaterialManager* s_instance;


  MaterialManager();
  ~MaterialManager();

  // Build special materials
  void buildSpecialMaterials();
};

#endif
