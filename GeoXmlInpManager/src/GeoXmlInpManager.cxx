/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecord.h"

#include "GeoModelXMLParser/XercesParser.h"
#include "GeoInpColDefHandler.h"
#include "GeoInpRowHandler.h"
#include "GeoInpTableDefHandler.h"
#include "GeoInpTableHandler.h"
#include "GeoInpDummyHandler.h"

#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>

#define PATH_ENV_NAME "GEOMODEL_XML_DIR"

namespace fs=std::filesystem;

GeoXmlInpManager* GeoXmlInpManager::s_instance{nullptr};

class Impl {
public:
  Impl(GeoXmlInpManager* man)
    : m_defsHandler("GeoInpDefs")
    , m_tabledefHandler("TableDef",man)
    , m_coldefHandler("ColDef",man)
    , m_tablesHandler("GeoInpTables")
    , m_tableHandler("Table",man)
    , m_rowHandler("Row",man)
  {
    m_currentTableDef = m_tableDefs.end();
    m_currentRecordset = m_recordsets.end();
  }
  ~Impl(){}

  //______________________________________________________
  // Handlers for the XML parser
  GeoInpDummyHandler    m_defsHandler;
  GeoInpTableDefHandler m_tabledefHandler;
  GeoInpColDefHandler   m_coldefHandler;
  GeoInpDummyHandler    m_tablesHandler;
  GeoInpTableHandler    m_tableHandler;
  GeoInpRowHandler      m_rowHandler;

  //______________________________________________________
  // Top of the XML input files directory tree
  fs::path m_path;

  //______________________________________________________
  // Container of table defs  
  std::map<std::string,GeoInpDef_ptr> m_tableDefs;
  std::map<std::string,GeoInpDef_ptr>::iterator m_currentTableDef;

  //______________________________________________________
  // Container of recordsets
  std::map<std::string,GeoInpRecordset_ptr> m_recordsets;
  std::map<std::string,GeoInpRecordset_ptr>::iterator m_currentRecordset;
};


GeoXmlInpManager::GeoXmlInpManager()
  : m_pImpl(new Impl(this))
{
  char* path = getenv(PATH_ENV_NAME);
  std::string errorMessage{""};
  if(!path) {
    errorMessage = std::string(PATH_ENV_NAME) + " environment not defined!";
    throw std::runtime_error(errorMessage.c_str());
  }

  fs::path fsPath(path);
  if(!fs::exists(fsPath)) {
    errorMessage = std::string(path) + " does not exist!";
    throw std::runtime_error(errorMessage.c_str());
  }
  m_pImpl->m_path = fsPath;  
}

GeoXmlInpManager::~GeoXmlInpManager()
{
  delete m_pImpl;
}

GeoXmlInpManager* GeoXmlInpManager::getManager()
{
  if(!s_instance) {
    try {
      s_instance = new GeoXmlInpManager();
    }
    catch(std::runtime_error& ex) {
      std::cerr << ex.what() << std::endl;
      return nullptr;
    }
  }
  return s_instance;
}

void GeoXmlInpManager::parse(const std::string& filename)
{
  fs::path path = m_pImpl->m_path / filename;
  if(!fs::exists(path)) {
    std::string errorMessage(std::string(path) + " does not exist!");
    throw std::runtime_error(errorMessage.c_str());
  }
  
  XercesParser xercesParser;
  xercesParser.ParseFileAndNavigate(path);
}

GeoInpRecordset_ptr GeoXmlInpManager::getRecordsetPtr(const std::string& nodeName)
{
  auto result = m_pImpl->m_recordsets.find(nodeName);
  if(result!=m_pImpl->m_recordsets.end()) {
    return result->second;
  }
  else {
    std::cout << "GeoXmlInpManager WARNING: " << nodeName << " does not exist. Returning empty recordset" << std::endl;
    return std::make_shared<GeoInpRecordset>(std::string(),std::make_shared<GeoInpDef>());
  }
}

void GeoXmlInpManager::addTableDef(const std::string& name)
{
  auto result = m_pImpl->m_tableDefs.insert(std::pair<std::string,GeoInpDef_ptr>(name,std::make_shared<GeoInpDef>()));
  if(result.second) {
    // New element has been inserted
    m_pImpl->m_currentTableDef = result.first;
  }
  else {
    std::string errorMessage("Attempted to redefine the existing table " + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void GeoXmlInpManager::addColDef(const std::string& name
				 , const std::string& type)
{
  if(m_pImpl->m_currentTableDef == m_pImpl->m_tableDefs.end()) {
    throw std::runtime_error("GeoXmlInpManager ERROR: addColDef called before defining any table!");
  }
  
  GeoInpDef_ptr inpDef = m_pImpl->m_currentTableDef->second;
  GeoInpType inpType{GEOINP_INT};
  if(type.compare("INT")==0) {
    inpType = GEOINP_INT;
  }
/*  else if(type.compare("LONG")==0) {
    inpType = GEOINP_LONG;
  }
  else if(type.compare("FLOAT")==0) {
    inpType = GEOINP_FLOAT;
  }  */
  else if(type.compare("DOUBLE")==0) {
    inpType = GEOINP_DOUBLE;
  }
  else if(type.compare("STRING")==0) {
    inpType = GEOINP_STRING;
  }
  else {
    std::string errorMessage("Unexpected type " + type + " requested for the Column " + name);
    throw std::runtime_error(errorMessage.c_str());
  }
  
  auto result = inpDef->insert(std::pair<std::string,GeoInpType>(name,inpType));
  if(!result.second) {
    std::string errorMessage("Attempted to redefine the existing column " + name + " in table " + m_pImpl->m_currentTableDef->first);
    throw std::runtime_error(errorMessage.c_str());
  }
}

void GeoXmlInpManager::addTable(const std::string& name)
{
  auto itDef = m_pImpl->m_tableDefs.find(name);
  if(itDef==m_pImpl->m_tableDefs.end()) {
    std::string errorMessage("Attempted to create recordset " + name + " without Table Def!");
    throw std::runtime_error(errorMessage.c_str());
  }

  GeoInpRecordset_ptr newRecordset = std::make_shared<GeoInpRecordset>(name,itDef->second);
  auto result = m_pImpl->m_recordsets.insert(std::pair<std::string,GeoInpRecordset_ptr>(name,newRecordset));
  if(result.second) {
    // New element has been inserted
    m_pImpl->m_currentRecordset = result.first;
  }
  else {
    std::string errorMessage("Attempted to re-fill the existing recordset " + name);
    throw std::runtime_error(errorMessage.c_str());
  }
}

GeoInpRecord& GeoXmlInpManager::addRecord()
{
  if(m_pImpl->m_currentRecordset == m_pImpl->m_recordsets.end()) {
    throw std::runtime_error("GeoXmlInpManager ERROR: addRecord called before creating any recordset!");
  }
  return m_pImpl->m_currentRecordset->second->addRecord();
}
