/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef INDETGEOMODELUTILS_TOPLEVELPLACEMENTS_H
#define INDETGEOMODELUTILS_TOPLEVELPLACEMENTS_H

#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecordset.h"
#include "GeoXmlInpManager/GeoInpRecord.h"
#include "GeoModelKernel/GeoDefinitions.h"

#include <map>
#include <string>

class GeoInpRecord;

class TopLevelPlacements
{
  
public:

  TopLevelPlacements(GeoInpRecordset_ptr topLevelTable);
  ~TopLevelPlacements();

  bool  present(const std::string & partName) const;
  const GeoTrf::Transform3D & transform(const std::string & partName) const;
  

private:
  
  class Part {
  public:
    std::string label;
    GeoTrf::Transform3D transform;
  };

  void fillPlacements(GeoInpRecordset_ptr topLevelTable);
  GeoTrf::Transform3D partTransform(const GeoInpRecord& record) const;
  Part * getPart(const std::string & partName) const;

  std::map<std::string, Part *> m_parts;
  bool m_noTopLevelTable;

  static const GeoTrf::Transform3D s_identityTransform;
};

#endif // INDETGEOMODELUTILS_TOPLEVELPLACEMENTS_H
