/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOINPRECORDSET_H
#define GEOXMLINPMANAGER_GEOINPRECORDSET_H

#include "GeoInpRecord.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

class GeoInpRecordset {
  friend class GeoXmlInpManager;
  
public:
  typedef std::vector<GeoInpRecord> RecordsVector;
  typedef RecordsVector::const_iterator const_iterator;

public:
  GeoInpRecordset(const std::string& nodeName
		  , std::shared_ptr<GeoInpDef> def)
    : m_nodeName(nodeName)
    , m_def(def)
  {}

  unsigned int size() const { return m_records.size(); }
  std::string nodeName() const { return m_nodeName; }
  const GeoInpRecord& operator[](unsigned int index) const { return m_records[index]; }
  const_iterator begin() const { return m_records.cbegin(); }
  const_iterator end() const { return m_records.cend(); }

  void dump() const;
protected:
  GeoInpRecord& addRecord();
  
private:
  std::string m_nodeName;
  std::shared_ptr<GeoInpDef> m_def;
  RecordsVector  m_records;
};

inline GeoInpRecord& GeoInpRecordset::addRecord()
{
  auto it = m_records.insert(m_records.end(),GeoInpRecord(m_def));
  return *it;
}

inline void GeoInpRecordset::dump() const
{
  std::cout << " *** GeoInpRecordset " << m_nodeName << " ***" << std::endl;
  std::cout << " *** *** *** *** *** *** *** *** *** *** ***" << std::endl;
  for(const GeoInpRecord& record : m_records) {
    record.dump();
  }
  std::cout << " *** *** *** *** *** *** *** *** *** *** ***" << std::endl;
}
#endif
