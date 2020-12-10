/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoMatComponentHandler.h"
#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include <iostream>

GeoMatComponentHandler::GeoMatComponentHandler(std::string s
					       , GeoXmlMatManager* matManager)
  : XMLHandler(s)
  , m_matManager(matManager)
{
}

void GeoMatComponentHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  double fraction=getAttributeAsDouble("fraction");

  m_matManager->addMatComponent(name,fraction);
}
