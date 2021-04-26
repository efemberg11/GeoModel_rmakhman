/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoNamespaceHandler.h"
#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include <iostream>

GeoNamespaceHandler::GeoNamespaceHandler(std::string s
					 ,GeoXmlMatManager* matManager)
  : XMLHandler(s)
  , m_matManager(matManager)
{
}

void GeoNamespaceHandler::ElementHandle()
{
  std::string name=getAttributeAsString("id");

  m_matManager->lockMaterial();
  m_matManager->addNamespace(name);
}
