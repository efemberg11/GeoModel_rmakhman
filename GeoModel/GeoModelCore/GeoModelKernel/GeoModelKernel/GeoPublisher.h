/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSTORE_H
#define GEOMODELKERNEL_GEOSTORE_H

/**
 * @class GeoPublisher
 *
 * @brief Class for a generic GeoModel publisher.
 * The interface allows to store pointers to two types of objects
 *   1. Full Physical Volumes
 *   2. Alignable Transforms
 * and to publish the lists.
 *
 * A pointer to a GeoPublisher object can be passed to GeoVGeometryPlugin::create().
 * This allows for storing pointers to full physical volumes and alignable transforms
 * while the plugin is building raw detector geometries. The stored pointers can
 * be used later for constructing the readout geometry layer, and for applying
 * alignment corrections
 */

// Author: Riccardo Maria Bianchi <riccardo.maria.bianchi@cern.ch> - Aug 2020
// Major updates: 
// - Jan 2021 - riccardo.maria.bianchi@cern.ch - Added support for XML auxiliary data


// C++ includes
#include <any> // needs C++17
#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <iostream>


class GeoAlignableTransform;
class GeoVFullPhysVol;

typedef std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::string>>> AuxTableDefs;
typedef std::unordered_map<std::string, std::vector<std::vector<std::variant<int,long,float,double,std::string>>>> AuxTableData;

class GeoPublisher
{
 public:
  GeoPublisher() = default;
  virtual ~GeoPublisher() = default;

  template<class N, typename T> void publishNode(N node,T keyT);

  std::map<GeoVFullPhysVol*, std::any> getPublishedFPV();
  std::map<GeoAlignableTransform*, std::any> getPublishedAXF();

  void setName(std::string name);
  std::string getName() { return m_name; }

  void storeDataTable( const std::string& tableName, const std::vector<std::string>& colNames, const std::vector<std::string>& colTypes, std::vector<std::vector<std::variant<int,long,float,double,std::string>>> tableData );

    std::pair<AuxTableDefs, AuxTableData> getPublishedAuxData() { return std::make_pair(m_auxiliaryTablesVar, m_auxiliaryTablesVarData); }

 private:

  std::map<GeoVFullPhysVol*, std::any> m_publishedFPV;
  std::map<GeoAlignableTransform*, std::any> m_publishedAXF;

  template<typename Iter> void printInsertionStatus(Iter it, bool success);

  std::string m_name;

  // cache to store custom tables to store auxiliary data in the DB:
    // ---> map( tableName, columnsNames, columnsTypes )
    AuxTableDefs m_auxiliaryTablesVar;
    AuxTableData m_auxiliaryTablesVarData;


}; 

// include the implementation of the class' templated member functions
#include "GeoPublisher.tpp"

#endif
