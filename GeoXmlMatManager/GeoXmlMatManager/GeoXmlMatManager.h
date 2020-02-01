/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLMATMANAGER_GEOXMLMATMANAGER_H
#define GEOXMLMATMANAGER_GEOXMLMATMANAGER_H

#include <iostream>

class GeoElement;
class GeoMaterial;
class Impl;

class GeoXmlMatManager {
  
  friend class GeoElementHandler;
  friend class GeoMatComponentHandler;
  friend class GeoMaterialHandler;
  friend class GeoNamespaceHandler;
  friend class Impl;
  
 public:
  static const GeoXmlMatManager* getManager();
  
  GeoMaterial *getMaterial(const std::string & name);
  const GeoMaterial *getMaterial(const std::string & name) const;

  GeoElement *getElement(const std::string & name);
  const GeoElement *getElement(const std::string & name) const;

  GeoElement *getElement(unsigned int atomicNumber);
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
  static GeoXmlMatManager* s_instance;

  GeoXmlMatManager();
  ~GeoXmlMatManager();

  // Build special materials
  void buildSpecialMaterials();
};

#endif
