/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoInpTableHandler.h"
#include "GeoXmlInpManager/GeoXmlInpManager.h"

GeoInpTableHandler::GeoInpTableHandler(std::string s
				       , GeoXmlInpManager* man)
  : XMLHandler(s)
  , m_inpManager(man)
{
}

void GeoInpTableHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  m_inpManager->addTable(name);
}
