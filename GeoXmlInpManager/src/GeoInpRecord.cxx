/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoXmlInpManager/GeoInpRecord.h"
#include <iostream>
#include <iomanip>

GeoInpRecord::GeoInpRecord(std::shared_ptr<GeoInpDef> def)
  : m_def(def)
{
}

bool GeoInpRecord::isFieldNull(const std::string& field) const
{
  if(m_record.find(field)!=m_record.end()) return false;
  if(m_def->find(field)==m_def->end()) throw std::runtime_error( "Wrong name for the field "+ field);
  return true;
}
  
int GeoInpRecord::getInt(const std::string& field) const
{
  auto recIt = checkField(field,GEOINP_INT);
  return std::get<int>(recIt->second);
}
/*
long GeoInpRecord::getLong(const std::string& field) const
{
  auto recIt = checkField(field,GEOINP_LONG);
  return std::get<long>(recIt->second);  
}

float GeoInpRecord::getFloat(const std::string& field) const
{
  auto recIt = checkField(field,GEOINP_FLOAT);
  return std::get<float>(recIt->second);
}
*/
double GeoInpRecord::getDouble(const std::string& field) const
{
  auto recIt = checkField(field,GEOINP_DOUBLE);
  return std::get<double>(recIt->second);
}

std::string GeoInpRecord::getString(const std::string& field) const
{
  auto recIt = checkField(field,GEOINP_STRING);
  return std::get<std::string>(recIt->second);
}

void GeoInpRecord::addValue(const std::string& field
			   , GeoInp value)
{
  auto result = m_record.insert(std::pair<std::string,GeoInp>(field,value));
  if(!result.second) {
    std::string errorMessage("Unexpected error when adding new value for the field " + field + ". Duplicate field name?");
    throw std::runtime_error(errorMessage.c_str());
  }
}

void GeoInpRecord::dump() const
{
  for(const auto& element : m_record) {
    auto defIt = m_def->find(element.first);
    std::cout << "[\"" << element.first << "\" :" << std::setprecision(10);
    switch(defIt->second) {
    case GEOINP_INT:
      std::cout << "INT " << std::get<int>(element.second) << "], ";
      break;
/*    case GEOINP_LONG:
      std::cout << "LONG " << std::get<long>(element.second) << "], ";
      break;
    case GEOINP_FLOAT:
      std::cout << "FLOAT " << std::get<float>(element.second) << "], ";
      break; */
    case GEOINP_DOUBLE:
      std::cout << "DOUBLE " << std::get<double>(element.second) << "], ";
      break;
    case GEOINP_STRING:
      std::cout << "STRING " << std::get<std::string>(element.second) << "], ";
      break;
    default:
      std::cout << "ERROR], ";
    }
  }
  std::cout << std::endl;
}
