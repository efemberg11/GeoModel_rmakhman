/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef IGEOMATMANAGER_IGEOMATMANAGER_H
#define IGEOMATMANAGER_IGEOMATMANAGER_H

#include <iostream>

class GeoElement;
class GeoMaterial;

class IGeoMaterialManager {
  
 public:
  
  virtual const GeoMaterial *getMaterial(const std::string & name) const =0;
  virtual const GeoElement *getElement(const std::string & name) const =0;
  virtual const GeoElement *getElement(unsigned int atomicNumber) const=0;

  virtual void addElement(const std::string &name
		  , const std::string &symbol
		  , double z
		  , double a) =0;

  virtual void addNamespace(const std::string &name) =0;

  virtual void addMaterial(const std::string &name
		   , double density) =0;

  virtual void addMatComponent(const std::string &name
		       , double fraction) =0;

  IGeoMaterialManager() {;}
  virtual ~IGeoMaterialManager() {;}

};

#endif
