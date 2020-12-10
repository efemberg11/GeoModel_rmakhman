/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "GeoMaterialHandler.h"
#include "GeoXmlMatManager/GeoXmlMatManager.h"
#include <iostream>

GeoMaterialHandler::GeoMaterialHandler(std::string s
				       ,GeoXmlMatManager* matManager)
  : XMLHandler(s)
  , m_matManager(matManager)
{
}

void GeoMaterialHandler::ElementHandle()
{
  std::string name=getAttributeAsString("name");
  double density=getAttributeAsDouble("density");

  m_matManager->lockMaterial();
  m_matManager->addMaterial(name,density);
}
