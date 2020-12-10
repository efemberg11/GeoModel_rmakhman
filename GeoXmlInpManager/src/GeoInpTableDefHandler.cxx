/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInpTableDefHandler.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"

GeoInpTableDefHandler::GeoInpTableDefHandler(std::string s
					     , GeoXmlInpManager* man)
  : XMLHandler(s)
  , m_inpManager(man)
{
}

void GeoInpTableDefHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  m_inpManager->addTableDef(name);
}
