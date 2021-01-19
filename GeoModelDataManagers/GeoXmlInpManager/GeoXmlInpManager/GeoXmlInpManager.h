/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLINPMANAGER_GEOXMLINPMANAGER_H
#define GEOXMLINPMANAGER_GEOXMLINPMANAGER_H

#include "GeoInpRecordset.h"
#include <memory>

class Impl;

typedef std::shared_ptr<GeoInpRecordset> GeoInpRecordset_ptr;

class GeoXmlInpManager {
  friend class GeoInpColDefHandler;
  friend class GeoInpTableDefHandler;
  friend class GeoInpRowHandler;
  friend class GeoInpTableHandler;
  friend class Impl;
  
 public:
  static GeoXmlInpManager* getManager();

  void parse(const std::string& filename);
  GeoInpRecordset_ptr getRecordsetPtr(const std::string& nodeName);
  std::map<std::string, std::vector<std::string>> getRecordsetData(const std::string& nodeName);

 protected:
  void addTable(const std::string& name);
  void addTableDef(const std::string& name);
  void addColDef(const std::string& name
		 , const std::string& type);
  GeoInpRecord& addRecord();

 private:
  Impl* m_pImpl;
  static GeoXmlInpManager* s_instance;

  GeoXmlInpManager();
  ~GeoXmlInpManager();
};

#endif
