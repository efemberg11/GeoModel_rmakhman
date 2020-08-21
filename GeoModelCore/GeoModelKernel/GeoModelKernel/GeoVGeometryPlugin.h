/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVGEOMETRYPLUGIN_H
#define GEOMODELKERNEL_GEOVGEOMETRYPLUGIN_H

/**
 * @class: GeoVGeometryPlugin
 *
 * @brief Abstact Base Class for a geometry plugin.
 *	  The geometry plugin builds the raw geometry
 */

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoStore.h"

class GeoPhysVol;
//class GeoVStore;

class GeoVGeometryPlugin 
{
 public:
  // no default constructor. We prohibit it in the 'private' session 
  GeoVGeometryPlugin(std::string name, GeoStore* store) : m_pluginName( name ), m_store(store) { m_store->setName(m_pluginName); } 
  GeoVGeometryPlugin(std::string name) : m_pluginName( name ) {}
  virtual ~GeoVGeometryPlugin();

  // Create the system.
  // Note: this is a pure virtual method, 
  //       so you need to implement it in your derived plugin class
  virtual void create (GeoPhysVol* world, GeoVStore* store=nullptr) = 0;

  // Get the plugin's name
  std::string getName() { return m_pluginName; };

  // Get the stor that publishes the GeoFullPhysVol and AlignableTransform nodes
  GeoVStore* getStore() {return m_store; };

 private:

  // we prohibit default constructor, copy constructor, and assignment operator
  GeoVGeometryPlugin()=delete;
  GeoVGeometryPlugin(const GeoVGeometryPlugin &right)=delete;
  GeoVGeometryPlugin & operator=(const GeoVGeometryPlugin &right)=delete;

  // the name of the plugin. 
  // It will be used to keep track of the FullPhysVol 
  // and AlignableTransform nodes used in the plugin.
  std::string m_pluginName;

  // a pointer to an instance of a class inheriting from GeoVStore
  GeoStore* m_store;

};

#endif
