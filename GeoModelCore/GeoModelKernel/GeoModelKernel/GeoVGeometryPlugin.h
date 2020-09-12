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
//#include "GeoModelKernel/GeoVStore.h" // for create()
#include "GeoModelKernel/GeoPublisher.h"  // for publishing and I/O methods in general

class GeoPhysVol;

class GeoVGeometryPlugin 
{
 public:
  GeoVGeometryPlugin() {};
  GeoVGeometryPlugin(std::string name, GeoPublisher* publisher) : m_pluginName( name ), m_publisher(publisher) { m_publisher->setName(m_pluginName); } 
  GeoVGeometryPlugin(std::string name) : m_pluginName( name ) {}
  virtual ~GeoVGeometryPlugin();

  // Create the system.
  // Note: this is a pure virtual method, 
  //       so you need to implement it in your derived plugin class
  virtual void create (GeoPhysVol* world, GeoPublisher* publisher=nullptr) = 0;

  // Get the plugin's name
  std::string getName() { return m_pluginName; };

  // Get the Publisher that publishes the lists of the GeoFullPhysVol and AlignableTransform nodes
  GeoPublisher* getPublisher() {return m_publisher; };

 private:

  // we prohibit default constructor, copy constructor, and assignment operator
  //GeoVGeometryPlugin()=delete;
  //GeoVGeometryPlugin(const GeoVGeometryPlugin &right)=delete;
  //GeoVGeometryPlugin & operator=(const GeoVGeometryPlugin &right)=delete;

  // the name of the plugin. 
  // It will be used to keep track of the FullPhysVol 
  // and AlignableTransform nodes used in the plugin.
  std::string m_pluginName;

  // a pointer to an instance of a class inheriting from GeoVStore
  GeoPublisher* m_publisher;

};

#endif
