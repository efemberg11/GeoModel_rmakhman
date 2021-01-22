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
#include <iomanip>
#include <iostream>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#define PATH_ENV_NAME "GEOMODEL_XML_DIR"

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
  std::string m_path;

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

  DIR* pDir = opendir(path);
  if(pDir) {
    closedir(pDir);
  }
  else {
    errorMessage = "Unable to open " + std::string(path) + " directory!";
    throw std::runtime_error(errorMessage.c_str());
  }
  
  m_pImpl->m_path = std::string(path);  
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
  std::string path = m_pImpl->m_path + "/" + filename;
  FILE* pFile = fopen(path.c_str(),"r");
  if(pFile) {
    fclose(pFile);
  }
  else {
    std::string errorMessage(path + " does not exist!");
    throw std::runtime_error(errorMessage.c_str());
  }

  XercesParser xercesParser;
  xercesParser.ParseFileAndNavigate(path);
}

std::pair<std::map<std::string, std::vector<std::string>>, std::vector<std::vector<GeoInp>>> GeoXmlInpManager::getRecordsetData(const std::string& nodeName)
{
    // the map storing the data to be returned by the function
    std::pair<std::map<std::string, std::vector<std::string>>, std::vector<std::vector<GeoInp>>> tableMap;
    
    // the vectors and map storing the table's columns' name and type
    std::map<std::string, std::vector<std::string>> colsMap;
    std::vector<std::string> colNames;
    std::vector<std::string> colTypes;

    /* //TODO: put this as a separate method as getTables() ??
    std::cout << "***TEST***\n";
    for( auto& entry : m_pImpl->m_tableDefs ) {
        std::string tableName = entry.first;
        GeoInpDef_ptr inpDefPtr  = entry.second;
        std::cout << "TableName: " << tableName << ", " << inpDefPtr << "\n";
        for( auto& def : *inpDefPtr ) {
            std::string colName = def.first;
            GeoInpType  colType = def.second;
            std::string colTypeStr;
            std::cout << colName << ", " << colType;
            if(colType == GEOINP_INT)    colTypeStr = "GEOINP_INT";
            if(colType == GEOINP_LONG)   colTypeStr = "GEOINP_LONG";
            if(colType == GEOINP_FLOAT)  colTypeStr = "GEOINP_FLOAT";
            if(colType == GEOINP_DOUBLE) colTypeStr = "GEOINP_DOUBLE";
            if(colType == GEOINP_STRING) colTypeStr = "GEOINP_STRING";
            std::cout << ", " << colTypeStr << "\n";
            col
        }
    }
    std::cout << "***TEST***\n";
    std::cout << std::endl;
    */

    // get the table's definition: columns' names and types
    GeoInpDef_ptr tableDefPtr = m_pImpl->m_tableDefs[nodeName];

    // a map to store indexes of the table's columns
    std::map<std::string, unsigned> colIndexes;
    
    // a map to store the columns' GeoInp type
    std::map<std::string, GeoInpType> colTypesMap;

    // loop over the table's columns
    for( auto& def : *tableDefPtr ) {
        std::string colName = def.first;
        GeoInpType  colType = def.second;
        std::string colTypeStr;
        if(colType == GEOINP_INT)    colTypeStr = "GEOINP_INT";
        if(colType == GEOINP_LONG)   colTypeStr = "GEOINP_LONG";
        if(colType == GEOINP_FLOAT)  colTypeStr = "GEOINP_FLOAT";
        if(colType == GEOINP_DOUBLE) colTypeStr = "GEOINP_DOUBLE";
        if(colType == GEOINP_STRING) colTypeStr = "GEOINP_STRING";
        colNames.push_back(colName);
        colTypes.push_back(colTypeStr);
        colIndexes[colName] = colNames.size() - 1;
        colTypesMap[colName] = colType;
    }

    // store the table's columns' names and types
    colsMap["colNames"] = colNames;
    colsMap["colTypes"] = colTypes;

    // debug
    for( unsigned ii=0; ii<colNames.size(); ++ii )
        std::cout << "col: " << colNames[ii] << "\n" ;

   
    // init the vector to contain the table's data
    std::vector<std::vector<GeoInp>> rowsData;

    // get number of columns, to init the vector storing row's data
    unsigned nCols = colNames.size();
    
    // get a pointer to the table's records (i.e., the table's data)
    GeoInpRecordset_ptr recordSet = getRecordsetPtr(nodeName);

    // loop over table rows
    for( const GeoInpRecord& record : *recordSet ) {
        std::vector<GeoInp> vFields(nCols, "NULL");
        std::map<std::string, GeoInp> recordMap = record.getRecord();
        // loop over row's items (i.e., entries for the table's columns)
        for( const auto& entry : recordMap ) {
            std::string fieldName = entry.first;
            GeoInp fieldValue    = entry.second; // that's a std::variant
            
            /*
            std::cout << "fieldName: " << fieldName << " - ";  
            std::string fieldValueStr = "NULL";
            // debug
            std::visit( [](auto&& arg){ std::cout << "fieldType: " << typeid(arg).name() 
                                                  << ", fieldVal: " 
                                                  << arg << " \n"; }, fieldValue );
            switch( colTypesMap[fieldName] ) {
                case GEOINP_INT:
                    fieldValueStr = std::to_string( std::get<int>(fieldValue) );
                    break;
                case GEOINP_LONG:
                    fieldValueStr = std::to_string( std::get<long>(fieldValue) );
                    break;
                case GEOINP_FLOAT:
                    fieldValueStr = std::to_string( std::get<float>(fieldValue) );
                    break;
                case GEOINP_DOUBLE:
                    fieldValueStr = std::to_string( std::get<double>(fieldValue) );
                    break;
                case GEOINP_STRING:
                    fieldValueStr = std::get<std::string>(fieldValue);
                    break;
            }
            std::cout << "fieldValueStr: " << fieldValueStr << "\n";
            */
            // replace the default "NULL" string value in the data vector with the value
            // at the index stored for the given column
            vFields[ colIndexes[fieldName] ] = fieldValue;
        }
        // store the data vector for the current row 
        rowsData.push_back( vFields );
    }

    // store the table's data
    tableMap = std::make_pair( colsMap, rowsData );

    return tableMap;
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
  else if(type.compare("LONG")==0) {
    inpType = GEOINP_LONG;
  }
  else if(type.compare("FLOAT")==0) {
    inpType = GEOINP_FLOAT;
  }  
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
