/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInpColDefHandler.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"

GeoInpColDefHandler::GeoInpColDefHandler(std::string s
					 , GeoXmlInpManager* man)
  : XMLHandler(s)
  , m_inpManager(man)
{
}

void GeoInpColDefHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  std::string type=getAttributeAsString("type");
  m_inpManager->addColDef(name,type);
}
