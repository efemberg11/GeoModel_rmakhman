/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOXMLMATMANAGER_GEOMATCOMPONENTHANDLER_H
#define GEOXMLMATMANAGER_GEOMATCOMPONENTHANDLER_H

#include "GeoModelXMLParser/XMLHandler.h"
#include <string>

class GeoXmlMatManager;

class GeoMatComponentHandler : public XMLHandler {
 public:
  GeoMatComponentHandler(std::string,GeoXmlMatManager*);
  void ElementHandle();
 private:
  GeoXmlMatManager* m_matManager;
};

#endif
