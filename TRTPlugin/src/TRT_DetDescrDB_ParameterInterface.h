/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef TRT_DETDESCRDB_PARAMETERINTERFACE_H
#define TRT_DETDESCRDB_PARAMETERINTERFACE_H


#include "TRTParameterInterface.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"

class TopLevelPlacements;

class TRT_DetDescrDB_ParameterInterface : public TRTParameterInterface {

public:

   
  // Only allowed constructor
  TRT_DetDescrDB_ParameterInterface();

  ~TRT_DetDescrDB_ParameterInterface();
  // delete copy c'tor
  TRT_DetDescrDB_ParameterInterface(const TRT_DetDescrDB_ParameterInterface &) = delete;
  // delete assignment
  TRT_DetDescrDB_ParameterInterface & operator=(const TRT_DetDescrDB_ParameterInterface &) = delete;
  //
  virtual const GeoTrf::Transform3D & partTransform(const std::string & partName) const;
  virtual bool partPresent(const std::string & partName) const;

  const std::string& name() const { 
    static const std::string n("TRT_GeoModel::TRT_DetDescrDB_ParameterInterface"); 
    return n;
  }

  GeoInpRecordset_ptr  scalingTable() const {return m_scalingTable;}

  void SetValues();

private:

  TopLevelPlacements * m_placements;
  GeoInpRecordset_ptr m_scalingTable;
};

#endif
