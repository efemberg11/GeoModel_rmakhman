/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoElementHandler.h"
#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include <iostream>

GeoElementHandler::GeoElementHandler(std::string s
				     ,GeoXmlMatManager* matManager)
  : XMLHandler(s)
  , m_matManager(matManager)
{
}

void GeoElementHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  std::string symbol=getAttributeAsString("symbol");
  double ca=getAttributeAsDouble("a");
  int cz=getAttributeAsInt("z");

  m_matManager->addElement(name,symbol,cz,ca);
}
