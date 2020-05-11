/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInpRowHandler.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"
#include "GeoXmlInpManager/GeoInpRecord.h"
#include <iomanip>

GeoInpRowHandler::GeoInpRowHandler(std::string s
				   , GeoXmlInpManager* man)
  : XMLHandler(s)
  , m_inpManager(man)
{
}

void GeoInpRowHandler::ElementHandle()
{
  GeoInpRecord& newRecord = m_inpManager->addRecord();
  GeoInpDef_ptr defPtr = newRecord.getDef();

  int intVal{0};
//  long longVal{0};
//  float floatVal{0.0};
  double doubleVal{0.0};
  std::string stringVal{""};
  
  for(const auto& defElement : *defPtr) {
    std::string fieldName = defElement.first;
    // Check for NULL values
    if(isAttribute(fieldName)) {
      GeoInpType fieldType = defElement.second;
      GeoInp val;
      switch(fieldType) {
      case GEOINP_INT:
	intVal = getAttributeAsInt(fieldName);
	val = intVal;
	break;
/*      case GEOINP_LONG:
	longVal = getAttributeAsLong(fieldName);
	val = longVal;
	break;
      case GEOINP_FLOAT:
	floatVal = getAttributeAsFloat(fieldName);
	val = floatVal;
	break;  */
      case GEOINP_DOUBLE:
	doubleVal = getAttributeAsDouble(fieldName);
	val = doubleVal;
	break;
      case GEOINP_STRING:
	stringVal = getAttributeAsString(fieldName);
	val = stringVal;
	break;
      default:
	throw std::runtime_error("Unexpected error when parsing field name=" + fieldName);
      }
      newRecord.addValue(fieldName,val);
    }
  }
}
